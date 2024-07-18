
#include "ESPUIControlMgr.h"

static Control::ControlId_t idCounter = 0;

_ESPUIcontrolMgr::_ESPUIcontrolMgr()
{
#ifdef ESP32
    ControlsSemaphore = xSemaphoreCreateMutex();
    xSemaphoreGive(ControlsSemaphore);
#endif // def ESP32
}

void _ESPUIcontrolMgr::RemoveToBeDeletedControls()
{
#ifdef ESP32
    xSemaphoreTake(ControlsSemaphore, portMAX_DELAY);
#endif // def ESP32

    ControlObject_t * PreviousControl = nullptr;
    ControlObject_t * CurrentControl = controls;

    while (nullptr != CurrentControl)
    {
        ControlObject_t * NextControl = CurrentControl->next;
        if (CurrentControl->ToBeDeleted())
        {
            if (CurrentControl == controls)
            {
                // this is the root control
                controls = NextControl;
            }
            else
            {
                PreviousControl->next = NextControl;
            }
            delete CurrentControl;
            CurrentControl = NextControl;
        }
        else
        {
            PreviousControl = CurrentControl;
            CurrentControl = NextControl;
        }
    }
#ifdef ESP32
    xSemaphoreGive(ControlsSemaphore);
#endif // def ESP32
}

Control* _ESPUIcontrolMgr::getControl(Control::ControlId_t id)
{
#ifdef ESP32
    xSemaphoreTake(ControlsSemaphore, portMAX_DELAY);
#endif // !def ESP32
    Control* Response = getControlNoLock(id);
#ifdef ESP32
    xSemaphoreGive(ControlsSemaphore);
#endif // !def ESP32
    return Response;
}

// WARNING: Anytime you walk the chain of controllers, the protection semaphore
//          MUST be locked. This function assumes that the semaphore is locked
//          at the time it is called. Make sure YOU locked it :)
Control* _ESPUIcontrolMgr::getControlNoLock(Control::ControlId_t id)
{
    return getControlObjectNoLock(id);
} // getControlNoLock

// WARNING: Anytime you walk the chain of controllers, the protection semaphore
//          MUST be locked. This function assumes that the semaphore is locked
//          at the time it is called. Make sure YOU locked it :)
_ESPUIcontrolMgr::ControlObject_t * _ESPUIcontrolMgr::getControlObjectNoLock(Control::ControlId_t id)
{
    ControlObject_t * Response = nullptr;
    ControlObject_t * CurrentControl = controls;

    while (nullptr != CurrentControl)
    {
        if (CurrentControl->GetId() == id)
        {
            if (!CurrentControl->ToBeDeleted())
            {
                Response = CurrentControl;
            }
            break;
        }
        CurrentControl = CurrentControl->next;
    }

    return Response;
} // getControlObjectNoLock

bool _ESPUIcontrolMgr::removeControl(Control::ControlId_t id)
{
    bool Response = false;

    Control* control = getControl(id);
    if (control)
    {
        Response = true;
        control->DeleteControl();
        controlCount--;
    }
#ifdef DEBUG_ESPUI
    else
    {
        // Serial.println(String("Could not Remove Control ") + String(id));
    }
#endif // def DEBUG_ESPUI

    return Response;
}


/*
Prepare a chunk of elements as a single JSON string. If the allowed number of elements is greater than the total
number this will represent the entire UI. More likely, it will represent a small section of the UI to be sent. The
client will acknowledge receipt by requesting the next chunk.
 */
uint32_t _ESPUIcontrolMgr::prepareJSONChunk(uint16_t startindex,
                                            JsonDocument & rootDoc,
                                            bool InUpdateMode,
                                            String FragmentRequestString,
                                            uint32_t CurrentSyncID)
{
#ifdef ESP32
    xSemaphoreTake(ControlsSemaphore, portMAX_DELAY);
#endif // def ESP32

    // Serial.println(String("prepareJSONChunk: Start.          InUpdateMode: ") + String(InUpdateMode));
    // Serial.println(String("prepareJSONChunk: Start.            startindex: ") + String(startindex));
    // Serial.println(String("prepareJSONChunk: Start. FragmentRequestString: '") + FragmentRequestString + "'");
    int elementcount = 0;
    uint32_t MaxMarshaledJsonSize = (!InUpdateMode) ? ESPUI.jsonInitialDocumentSize: ESPUI.jsonUpdateDocumentSize;
    uint32_t EstimatedUsedMarshaledJsonSize = 0;

    do // once
    {
        // Follow the list until control points to the startindex'th node
        ControlObject_t * CurrentControlObject = controls;
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
            AllocateJsonDocument(FragmentRequest, FragmentRequestString.length() * 3);
/*
            ArduinoJson::detail::sizeofObject(N);
            if(0 >= FragmentRequest.capacity())
            {
                Serial.println(F("ERROR:prepareJSONChunk:Fragmentation:Could not allocate memory for a fragmentation request. Skipping Response"));
                break;
            }
*/
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
            CurrentControlObject = getControlObjectNoLock(ControlId);
            if(nullptr == CurrentControlObject)
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
        while ((startindex > currentIndex) && (nullptr != CurrentControlObject))
        {
            // only count active controls
            if (!CurrentControlObject->ToBeDeleted())
            {
                if(InUpdateMode)
                {
                    // In update mode we only count the controls that have been updated.
                    if(CurrentControlObject->NeedsSync(CurrentSyncID))
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
            CurrentControlObject = CurrentControlObject->next;
        }

        // any controls left to be processed?
        if(nullptr == CurrentControlObject)
        {
            // Serial.println("prepareJSONChunk: No controls to process");
            break;
        }

        // keep track of the number of elements we have serialised into this
        // message. Overflow is detected and handled later in this loop
        // and needs an index to the last item added.
        while (nullptr != CurrentControlObject)
        {
            // skip deleted controls or controls that have not been updated
            if (CurrentControlObject->ToBeDeleted() && !SingleControl)
            {
                // Serial.println(String("prepareJSONChunk: Ignoring Deleted control: ") + String(control->id));
                CurrentControlObject = CurrentControlObject->next;
                continue;
            }

            if(InUpdateMode && !SingleControl)
            {
                if(CurrentControlObject->NeedsSync(CurrentSyncID))
                {
                    // dont skip this control
                }
                else
                {
                    // control has not been updated. Skip it
                    CurrentControlObject = CurrentControlObject->next;
                    continue;
                }
            }

            // Serial.println(String(F("prepareJSONChunk: MaxMarshaledJsonSize: ")) + String(MaxMarshaledJsonSize));
            // Serial.println(String(F("prepareJSONChunk: Cur EstimatedUsedMarshaledJsonSize: ")) + String(EstimatedUsedMarshaledJsonSize));

            JsonObject item = AllocateJsonObject(items);
            elementcount++;
            uint32_t RemainingSpace = (MaxMarshaledJsonSize - EstimatedUsedMarshaledJsonSize) - 100;
            // Serial.println(String(F("prepareJSONChunk: RemainingSpace: ")) + String(RemainingSpace));
            uint32_t SpaceUsedByMarshaledControl = 0;
            bool ControlIsFragmented = CurrentControlObject->MarshalControl(item,
                                                               InUpdateMode,
                                                               DataOffset,
                                                               RemainingSpace,
                                                               SpaceUsedByMarshaledControl);
            // Serial.println(String(F("prepareJSONChunk: SpaceUsedByMarshaledControl: ")) + String(SpaceUsedByMarshaledControl));
            EstimatedUsedMarshaledJsonSize += SpaceUsedByMarshaledControl;
            // Serial.println(String(F("prepareJSONChunk: New EstimatedUsedMarshaledJsonSize: ")) + String(EstimatedUsedMarshaledJsonSize));
            // Serial.println(String(F("prepareJSONChunk:                ControlIsFragmented: ")) + String(ControlIsFragmented));

            // did the control get added to the doc?
            if (0 == SpaceUsedByMarshaledControl ||
                (ESPUI.jsonChunkNumberMax > 0 && (elementcount % ESPUI.jsonChunkNumberMax) == 0))
            {
                // Serial.println( String("prepareJSONChunk: too much data in the message. Remove the last entry"));
                if (1 == elementcount)
                {
                    // Serial.println(String(F("prepareJSONChunk: Control ")) + String(control->id) + F(" is too large to be sent to the browser."));
                    // Serial.println(String(F("ERROR: prepareJSONChunk: value: ")) + control->value);
                    rootDoc.clear();
                    item = AllocateJsonObject(items);
                    CurrentControlObject->MarshalErrorMessage(item);
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
                CurrentControlObject = nullptr;
            }
            else if ((SingleControl) ||
                     (ControlIsFragmented) ||
                     (MaxMarshaledJsonSize < (EstimatedUsedMarshaledJsonSize + 100)))
            {
                // Serial.println("prepareJSONChunk: Doc is Full, Fragmented Control or Single Control. exit loop");
                CurrentControlObject = nullptr;
            }
            else
            {
                // Serial.println("prepareJSONChunk: Next Control");
                CurrentControlObject = CurrentControlObject->next;
            }
        } // end while (control != nullptr)

    } while (false);

#ifdef ESP32
    xSemaphoreGive(ControlsSemaphore);
#endif // def ESP32

    // Serial.println(String("prepareJSONChunk: END: elementcount: ") + String(elementcount));
    return elementcount;
}


Control::ControlId_t _ESPUIcontrolMgr::addControl(Control::Type type,
                                                  const char* label,
                                                  const String& value,
                                                  Control::Color color,
                                                  Control::ControlId_t parentControl,
                                                  bool visible,
                                                  std::function<void(Control*, int)> callback)
{
    // Create a Wrapper and a control
    ControlObject_t * NewControlObject  = new ControlObject_t(++idCounter, type, label, callback, value, color, visible, parentControl);
    NewControlObject->next = nullptr;

#ifdef ESP32
    xSemaphoreTake(ControlsSemaphore, portMAX_DELAY);
#endif // def ESP32

    if (controls == nullptr)
    {
        controls = NewControlObject;
    }
    else
    {
        ControlObject_t * iterator = controls;
        while (iterator->next != nullptr)
        {
            iterator = iterator->next;
        }
        iterator->next = NewControlObject;
    }

    controlCount++;

#ifdef ESP32
    xSemaphoreGive(ControlsSemaphore);
#endif // def ESP32

    return NewControlObject->GetId();
}

_ESPUIcontrolMgr::ControlObject_t::ControlObject_t(Control::ControlId_t id, Control::Type type, const char* label, std::function<void(Control*, int)> callback,
    const String& value, Control::Color color, bool visible, ControlId_t parentControl)
    : Control(id, type, label, callback, value, color, visible, parentControl)
{}

// Instantiate the singleton
_ESPUIcontrolMgr ESPUIcontrolMgr;
