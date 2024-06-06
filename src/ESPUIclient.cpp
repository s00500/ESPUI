#include "ESPUI.h"
#include "ESPUIclient.h"
#include "ESPUIcontrol.h"

// JSONSlave:
// helper to process exact JSON serialization size
// it takes ~2ms on esp8266 and avoid large String reallocation which is really worth the cost
class JSONSlave: public Print
{
public:
    size_t write (uint8_t c) override { counter++; return 1; }
    size_t write (const uint8_t* buf, size_t count) override { counter += count; return count; }
    size_t get_counter () { return counter; }

    static size_t serializedSize (JsonDocument& doc)
    {
        JSONSlave counter;
        serializeJson(doc, counter);
        return counter.get_counter();
    }

    static size_t serialize (JsonDocument& doc, String& str)
    {
        size_t s = serializedSize(doc) + 10; // 10 is paranoid
        str.reserve(s);
        serializeJson(doc, str);
        return s;
    }

    static String toString (JsonDocument& doc)
    {
        String str;
        serialize(doc, str);
        return str;
    }

protected:
    size_t counter = 0;
};

ESPUIclient::ESPUIclient(AsyncWebSocketClient * _client):
    client(_client)
{
    fsm_EspuiClient_state_Idle_imp.SetParent(this);
    fsm_EspuiClient_state_SendingUpdate_imp.SetParent(this);
    fsm_EspuiClient_state_Rebuilding_imp.SetParent(this);
    fsm_EspuiClient_state_Reloading_imp.SetParent(this);

    fsm_EspuiClient_state_Idle_imp.Init();
}

ESPUIclient::ESPUIclient(const ESPUIclient& source):
    client(source.client)
{
    fsm_EspuiClient_state_Idle_imp.SetParent(this);
    fsm_EspuiClient_state_SendingUpdate_imp.SetParent(this);
    fsm_EspuiClient_state_Rebuilding_imp.SetParent(this);
    fsm_EspuiClient_state_Reloading_imp.SetParent(this);

    fsm_EspuiClient_state_Idle_imp.Init();
}

ESPUIclient::~ESPUIclient()
{
}

bool ESPUIclient::CanSend()
{
    bool Response = false;
    if (nullptr != client)
    {
        Response = client->canSend();
    }
    return Response;
}

#if ARDUINOJSON_VERSION_MAJOR < 7
void ESPUIclient::FillInHeader(DynamicJsonDocument& document)
#else
void ESPUIclient::FillInHeader(JsonDocument& document)
#endif
{
    document[F("type")] = UI_EXTEND_GUI;
    document[F("sliderContinuous")] = ESPUI.sliderContinuous;
    document[F("startindex")] = 0;
    document[F("totalcontrols")] = ESPUI.controlCount;
    JsonArray items = document.createNestedArray(F("controls"));
    JsonObject titleItem = items.createNestedObject();
    titleItem[F("type")] = (int)UI_TITLE;
    titleItem[F("label")] = ESPUI.ui_title;
}

bool ESPUIclient::IsSyncronized()
{
    return ((ClientUpdateType_t::Synchronized == ClientUpdateType) &&
            (&fsm_EspuiClient_state_Idle_imp == pCurrentFsmState));
}

bool ESPUIclient::SendClientNotification(ClientUpdateType_t value)
{
    bool Response = false;

    do // once
    {
        if(!CanSend())
        {
            // Serial.println(F("ESPUIclient::SendClientNotification:CannotSend"));
            break;
        }
#if ARDUINOJSON_VERSION_MAJOR < 7
        DynamicJsonDocument document(ESPUI.jsonUpdateDocumentSize);
#else
        JsonDocument document;
#endif     
        FillInHeader(document);
        if(ClientUpdateType_t::ReloadNeeded == value)
        {
            // Serial.println(F("ESPUIclient::SendClientNotification:set type to reload"));
            document["type"] = int(UI_RELOAD);
        }
        // dont send any controls

        Response = SendJsonDocToWebSocket(document);
        // Serial.println(String("ESPUIclient::SendClientNotification:NotificationSent:Response: ") + String(Response));

    } while (false);
    return Response;
}

void ESPUIclient::NotifyClient(ClientUpdateType_t newState)
{
    SetState(newState);
    pCurrentFsmState->NotifyClient();
}

// Handle Websockets Communication
bool ESPUIclient::onWsEvent(AwsEventType type, void* arg, uint8_t* data, size_t len)
{
    bool Response = false;
    // Serial.println(String("ESPUIclient::OnWsEvent: type: ") + String(type));

    switch (type)
    {
        case WS_EVT_PONG:
        {
            #if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.println(F("ESPUIclient::OnWsEvent:WS_EVT_PONG"));
            }
            #endif
            break;
        }

        case WS_EVT_ERROR:
        {
            #if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.println(F("ESPUIclient::OnWsEvent:WS_EVT_ERROR"));
            }
            #endif
            break;
        }

        case WS_EVT_CONNECT:
        {
    		#if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.println(F("ESPUIclient::OnWsEvent:WS_EVT_CONNECT"));
                Serial.println(client->id());
            }
    		#endif

            // Serial.println("ESPUIclient:onWsEvent:WS_EVT_CONNECT: Call NotifyClient: RebuildNeeded");
            NotifyClient(ClientUpdateType_t::RebuildNeeded);
            break;
        }

        case WS_EVT_DATA:
        {
            // Serial.println(F("ESPUIclient::OnWsEvent:WS_EVT_DATA"));
            String msg = "";
            msg.reserve(len + 1);

            for (size_t i = 0; i < len; i++)
            {
                msg += (char)data[i];
            }

            String   cmd   = msg.substring(0, msg.indexOf(":"));
            String   value = msg.substring(cmd.length() + 1, msg.lastIndexOf(':'));
            uint16_t id    = msg.substring(msg.lastIndexOf(':') + 1).toInt();

            #if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity >= Verbosity::VerboseJSON)
                {
                    Serial.println(String(F("  WS msg: ")) + msg);
                    Serial.println(String(F("  WS cmd: ")) + cmd);
                    Serial.println(String(F("   WS id: ")) + String(id));
                    Serial.println(String(F("WS value: ")) + String(value));
                }
            #endif

            if (cmd.equals(F("uiok")))
            {
                
                // Serial.println(String(F("ESPUIclient::OnWsEvent:WS_EVT_DATA:uiok:ProcessAck:")) + pCurrentFsmState->GetStateName());
                pCurrentFsmState->ProcessAck(id, emptyString);
                break;
            }

            if (cmd.equals(F("uifragmentok")))
            {
                // Serial.println(String(F("ESPUIclient::OnWsEvent:WS_EVT_DATA:uiok:uifragmentok:")) + pCurrentFsmState->GetStateName() + ":ProcessAck");
                if(!emptyString.equals(value))
                {
                    // Serial.println(String(F("ESPUIclient::OnWsEvent:WS_EVT_DATA:uiok:uifragmentok:")) + pCurrentFsmState->GetStateName() + ":ProcessAck:value:'" +  value + "'");
                    pCurrentFsmState->ProcessAck(uint16_t(-1), value);
                }
                else
                {
                    Serial.println(F("ERROR:ESPUIclient::OnWsEvent:WS_EVT_DATA:uifragmentok:ProcessAck:Fragment Header is missing"));
                }
                break;
            }

            if (cmd.equals(F("uiuok")))
            {
                // Serial.println(F("WS_EVT_DATA: uiuok. Unlock new async notifications"));
                break;
            }

            // Serial.println(F("WS_EVT_DATA:Process Control"));
            Control* control = ESPUI.getControl(id);
            if (nullptr == control)
            {
                #if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity)
                {
                    Serial.println(String(F("No control found for ID ")) + String(id));
                }
                #endif
                break;
            }
            control->onWsEvent(cmd, value);
            // notify other clients of change
            Response = true;
            break;
        }

        default:
        {
            // Serial.println(F("ESPUIclient::OnWsEvent:default"));
            break;
        }
    } // end switch

    return Response;
}

/*
Prepare a chunk of elements as a single JSON string. If the allowed number of elements is greater than the total
number this will represent the entire UI. More likely, it will represent a small section of the UI to be sent. The
client will acknowledge receipt by requesting the next chunk.
 */
#if ARDUINOJSON_VERSION_MAJOR < 7
uint32_t ESPUIclient::prepareJSONChunk(uint16_t startindex,
                                      DynamicJsonDocument & rootDoc,
                                      bool InUpdateMode,
                                      String FragmentRequestString)
#else
uint32_t ESPUIclient::prepareJSONChunk(uint16_t startindex,
                                      JsonDocument & rootDoc,
                                      bool InUpdateMode,
                                      String FragmentRequestString)
#endif
{
#ifdef ESP32
    xSemaphoreTake(ESPUI.ControlsSemaphore, portMAX_DELAY);
#endif // def ESP32

    // Serial.println(String("prepareJSONChunk: Start. InUpdateMode: ") + String(InUpdateMode));
    int elementcount = 0;

    do // once
    {
        // Follow the list until control points to the startindex'th node
        Control* control = ESPUI.controls;
        uint32_t currentIndex = 0;
        uint32_t DataOffset = 0;
        JsonArray items = rootDoc[F("controls")];
        bool SingleControl = false;

        if(!emptyString.equals(FragmentRequestString))
        {
            // Serial.println(F("prepareJSONChunk:Fragmentation:Got Header (1)"));
            // Serial.println(String("prepareJSONChunk:startindex:                  ") + String(startindex));
            // Serial.println(String("prepareJSONChunk:currentIndex:                ") + String(currentIndex));
            // Serial.println(String("prepareJSONChunk:FragmentRequestString:      '") + FragmentRequestString + "'");

            // this is actually a fragment or directed update request
            // parse the string we got from the UI and try to update that specific 
            // control.
#if ARDUINOJSON_VERSION_MAJOR < 7
            DynamicJsonDocument FragmentRequest(FragmentRequestString.length() * 3);
            if(0 >= FragmentRequest.capacity())
            {
                Serial.println(F("ERROR:prepareJSONChunk:Fragmentation:Could not allocate memory for a fragmentation request. Skipping Response"));
                break;
            }
#else
            JsonDocument FragmentRequest;
#endif
            size_t FragmentRequestStartOffset = FragmentRequestString.indexOf("{");
            DeserializationError error = deserializeJson(FragmentRequest, FragmentRequestString.substring(FragmentRequestStartOffset));
            if(DeserializationError::Ok != error)
            {
                Serial.println(F("ERROR:prepareJSONChunk:Fragmentation:Could not extract json from the fragment request"));
                break;
            }

            if(!FragmentRequest.containsKey(F("id")))
            {
                Serial.println(F("ERROR:prepareJSONChunk:Fragmentation:Request does not contain a control ID"));
                break;
            }
            uint16_t ControlId = uint16_t(FragmentRequest[F("id")]);

            if(!FragmentRequest.containsKey(F("offset")))
            {
                Serial.println(F("ERROR:prepareJSONChunk:Fragmentation:Request does not contain a starting offset"));
                break;
            }
            DataOffset = uint16_t(FragmentRequest[F("offset")]);
            control = ESPUI.getControlNoLock(ControlId);
            if(nullptr == control)
            {
                Serial.println(String(F("ERROR:prepareJSONChunk:Fragmentation:Requested control: ")) + String(ControlId) + F(" does not exist"));
                break;
            }

            // Serial.println(F("prepareJSONChunk:Fragmentation:disable the control search operation"));
            currentIndex = 1;
            startindex = 0;
            SingleControl = true;
        }

        // find a control to send
        while ((startindex > currentIndex) && (nullptr != control))
        {
            // only count active controls
            if (!control->ToBeDeleted())
            {
                if(InUpdateMode)
                {
                    // In update mode we only count the controls that have been updated.
                    if(control->NeedsSync(CurrentSyncID))
                    {
                        ++currentIndex;
                    }
                }
                else
                {
                    // not in update mode. Count all active controls
                    ++currentIndex;
                }
            }
            control = control->next;
        }

        // any controls left to be processed?
        if(nullptr == control)
        {
            // Serial.println("prepareJSONChunk: No controls to process");
            break;
        }

        // keep track of the number of elements we have serialised into this
        // message. Overflow is detected and handled later in this loop
        // and needs an index to the last item added.
        while (nullptr != control)
        {
            // skip deleted controls or controls that have not been updated
            if (control->ToBeDeleted() && !SingleControl)
            {
                // Serial.println(String("prepareJSONChunk: Ignoring Deleted control: ") + String(control->id));
                control = control->next;
                continue;
            }

            if(InUpdateMode && !SingleControl)
            {
                if(control->NeedsSync(CurrentSyncID))
                {
                    // dont skip this control
                }
                else
                {
                    // control has not been updated. Skip it
                    control = control->next;
                    continue;
                }
            }

            JsonObject item = items.createNestedObject();
            elementcount++;
            control->MarshalControl(item, InUpdateMode, DataOffset);
            
            if (rootDoc.overflowed() || (ESPUI.jsonChunkNumberMax > 0 && (elementcount % ESPUI.jsonChunkNumberMax) == 0))
            {
                // String("prepareJSONChunk: too much data in the message. Remove the last entry");
                if (1 == elementcount)
                {
                    Serial.println(String(F("ERROR: prepareJSONChunk: Control ")) + String(control->id) + F(" is too large to be sent to the browser."));
                    // Serial.println(String(F("ERROR: prepareJSONChunk: value: ")) + control->value);
                    rootDoc.clear();
                    item = items.createNestedObject();
                    control->MarshalErrorMessage(item);
                    elementcount = 0;
                }
                else
                {
                    // Serial.println(String("prepareJSONChunk: Defering control: ") + String(control->id));
                    // Serial.println(String("prepareJSONChunk: elementcount: ") + String(elementcount));

                    items.remove(elementcount);
                    --elementcount;
                }
                // exit the loop
                control = nullptr;
            }
            else if (SingleControl)
            {
                // Serial.println("prepareJSONChunk: exit loop");
                control = nullptr;
            }
            else
            {
                control = control->next;
            }
        } // end while (control != nullptr)

    } while (false);

#ifdef ESP32
    xSemaphoreGive(ESPUI.ControlsSemaphore);
#endif // def ESP32

    // Serial.println(String("prepareJSONChunk: elementcount: ") + String(elementcount));
    return elementcount;
}

/*
Convert & Transfer Arduino elements to JSON elements. This function sends a chunk of
JSON describing the controls of the UI, starting from the control at index startidx.
If startidx is 0 then a UI_INITIAL_GUI message will be sent, else a UI_EXTEND_GUI.
Both message types contain a list of serialised UI elements. Only a portion of the UI
will be sent in order to avoid websocket buffer overflows. The client will acknowledge
receipt of a partial message by requesting the next chunk of UI.

The protocol is:
SERVER: SendControlsToClient(0):
    "UI_INITIAL_GUI: n serialised UI elements"
CLIENT: controls.js:handleEvent()
    "uiok:n"
SERVER: SendControlsToClient(n):
    "UI_EXTEND_GUI: n serialised UI elements"
CLIENT: controls.js:handleEvent()
    "uiok:2*n"
etc.
    Returns true if all controls have been sent (aka: Done)
*/
bool ESPUIclient::SendControlsToClient(uint16_t startidx, ClientUpdateType_t TransferMode, String FragmentRequest)
{
    bool Response = false;
    // Serial.println(String("ESPUIclient:SendControlsToClient:startidx: ") + String(startidx));
    do // once
    {
        if(!CanSend())
        {
            // Serial.println("ESPUIclient:SendControlsToClient: Cannot Send to clients.");
            break;
        }

        else if ((startidx >= ESPUI.controlCount) && (emptyString.equals(FragmentRequest)))
        {
            // Serial.println(F("ERROR:ESPUIclient:SendControlsToClient: No more controls to send."));
            Response = true;
            break;
        }

#if ARDUINOJSON_VERSION_MAJOR < 7
        DynamicJsonDocument document(ESPUI.jsonInitialDocumentSize);
#else
        JsonDocument document;
#endif
        FillInHeader(document);
        document[F("startindex")] = startidx;
        document[F("totalcontrols")] = uint16_t(-1); // ESPUI.controlCount;

        if(0 == startidx)
        {
            // Serial.println("ESPUIclient:SendControlsToClient: Tell client we are starting a transfer of controls.");
            document["type"] = (ClientUpdateType_t::RebuildNeeded == TransferMode) ? UI_INITIAL_GUI : UI_EXTEND_GUI;
            CurrentSyncID = NextSyncID;
            NextSyncID = ESPUI.GetNextControlChangeId();
        }
        // Serial.println(String("ESPUIclient:SendControlsToClient:type: ") + String((uint32_t)document["type"]));

        // Serial.println("ESPUIclient:SendControlsToClient: Build Controls.");
        if(prepareJSONChunk(startidx, document, ClientUpdateType_t::UpdateNeeded == TransferMode, FragmentRequest))
        {
            #if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity >= Verbosity::VerboseJSON)
                {
                    Serial.println(F("ESPUIclient:SendControlsToClient: Sending elements --------->"));
                    serializeJson(document, Serial);
                    Serial.println();
                }
            #endif

            // Serial.println("ESPUIclient:SendControlsToClient: Send message.");
            if(true == SendJsonDocToWebSocket(document))
            {
                // Serial.println("ESPUIclient:SendControlsToClient: Sent.");
            }
            else
            {
                // Serial.println("ESPUIclient:SendControlsToClient: Send failed.");
            }
        }
        else
        {
            // Serial.println("ESPUIclient:SendControlsToClient: No elements to send.");
            Response = true;
        }

    } while(false);

    // Serial.println(String("ESPUIclient:SendControlsToClient:Response: ") + String(Response));
    return Response;
}

#if ARDUINOJSON_VERSION_MAJOR < 7
bool ESPUIclient::SendJsonDocToWebSocket(DynamicJsonDocument& document)
#else
bool ESPUIclient::SendJsonDocToWebSocket(JsonDocument& document)
#endif
{
    bool Response = true;

    do // once
    {
        if (!CanSend())
        {
            #if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity >= Verbosity::VerboseJSON)
                {
                    Serial.println(F("ESPUIclient::SendJsonDocToWebSocket: Cannot Send to client. Not sending websocket message"));
                }
            #endif
            // Serial.println("ESPUIclient::SendJsonDocToWebSocket: Cannot Send to client. Not sending websocket message");
            Response = false;
            break;
        }

        String json = JSONSlave::toString(document);

        #if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity >= Verbosity::VerboseJSON)
            {
                Serial.println(String(F("ESPUIclient::SendJsonDocToWebSocket: json: '")) + json + "'");
            }
        #endif

        #if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity >= Verbosity::VerboseJSON)
            {
                Serial.println(F("ESPUIclient::SendJsonDocToWebSocket: client.text"));
            }
        #endif
        // Serial.println(F("ESPUIclient::SendJsonDocToWebSocket: client.text"));
        client->text(json);

    } while (false);

    return Response;
}

void ESPUIclient::SetState(ClientUpdateType_t value)
{
    // only a higher priority state request can replace the current state request
    if(uint32_t(ClientUpdateType) < uint32_t(value))
    {
        ClientUpdateType = value;
    }
}

