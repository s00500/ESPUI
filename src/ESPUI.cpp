#include "ESPUI.h"

#include <functional>

#include <ESPAsyncWebServer.h>

#include "dataControlsJS.h"
#include "dataGraphJS.h"
#include "dataIndexHTML.h"
#include "dataNormalizeCSS.h"
#include "dataSliderJS.h"
#include "dataStyleCSS.h"
#include "dataTabbedcontentJS.h"
#include "dataZeptoJS.h"

#if ESP8266
#include <umm_malloc/umm_heap_select.h>
#endif

static String heapInfo(const __FlashStringHelper* mode)
{
    String result;
#if ESP8266

    uint32_t hfree;
    uint16_t hmax;
    uint8_t hfrag;
    result.reserve(128);

#ifdef UMM_HEAP_IRAM
    // here esp8266 is configurerd to use an extra 16KB (i)ram
    {
        HeapSelectIram useInstructionRamHere;
        ESP.getHeapStats(&hfree, &hmax, &hfrag);
    }
    result += F("IRAM: free: ");
    result += hfree;
    result += F(" max: ");
    result += hmax;
    result += F(" frag: ");
    result += hfrag;
    result += "%\n";
#endif // !UMM_HEAP_IRAM
    {
        HeapSelectDram useRegularRamHere;
        ESP.getHeapStats(&hfree, &hmax, &hfrag);
    }
    result += F("DRAM: free: ");
    result += hfree;
    result += F(" max: ");
    result += hmax;
    result += F(" frag: ");
    result += hfrag;
    result += "%\n";

#else // !ESP8266

    result += ESP.getFreeHeap();
    result += ' ';

#endif // !ESP8266

    result += mode;

    return result;
}

// ################# LITTLEFS functions
#if defined(ESP32)
void listDir(const char* dirname, uint8_t levels)
{
#if defined(DEBUG_ESPUI)
    if (ESPUI.verbosity)
    {
        Serial.printf_P(PSTR("Listing directory: %s\n"), dirname);
    }
#endif

    File root = ESPUI.EspuiLittleFS.open(dirname);
    if (!root)
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("Failed to open directory"));
        }
#endif

        return;
    }

    if (!root.isDirectory())
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("Not a directory"));
        }
#endif

        return;
    }

    File file = root.openNextFile();

    while (file)
    {
        if (file.isDirectory())
        {
#if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.print(F("  DIR : "));
                Serial.println(file.name());
            }
#endif

            if (levels)
            {
#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
                listDir(file.path(), levels - 1);
#else
                listDir(file.name(), levels - 1);
#endif
            }
        }
        else
        {
#if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.print(F("  FILE: "));
                Serial.print(file.name());
                Serial.print(F("  SIZE: "));
                Serial.println(file.size());
            }
#endif
        }

        file = root.openNextFile();
    }
}
#else

void listDir(const char* dirname, uint8_t levels)
{
#if defined(DEBUG_ESPUI)
    if (ESPUI.verbosity)
    {
        Serial.printf_P(PSTR("Listing directory: %s\n"), dirname);
    }
#endif

    Dir dir = ESPUI.EspuiLittleFS.openDir(dirname);

    while (dir.next())
    {
        if (dir.isDirectory())
        {
#if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.print(F("  DIR : "));
                Serial.println(dir.fileName());
            }
#endif
            if (levels)
            {
                File file = dir.openFile("r");
                listDir(file.fullName(), levels - 1);
                file.close();
            }
        }
        else
        {
#if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.print(F("  FILE: "));
                Serial.print(dir.fileName());
                Serial.print(F("  SIZE: "));
                Serial.println(dir.fileSize());
            }
#endif
        }
    }
}

#endif

void ESPUIClass::list()
{
    if (!EspuiLittleFS.begin())
    {
        Serial.println(F("Espui LittleFS Mount Failed"));
        return;
    }

    listDir("/", 1);

#if defined(ESP32)
    Serial.print(F("Total KB: "));
    Serial.println(EspuiLittleFS.totalBytes() / 1024);
    Serial.print(F("Used KB: "));
    Serial.println(EspuiLittleFS.usedBytes() / 1024);
#else
    FSInfo fs_info;
    EspuiLittleFS.info(fs_info);

    Serial.print(F("Total KB: "));
    Serial.println(fs_info.totalBytes / 1024);
    Serial.print(F("Used KB: "));
    Serial.println(fs_info.usedBytes / 1024);
#endif // !defined(ESP32)
}

void deleteFile(const char* path)
{
    bool exists = ESPUI.EspuiLittleFS.exists(path);
    if (!exists)
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.printf_P(PSTR("File: %s does not exist, not deleting\n"), path);
        }
#endif

        return;
    }

#if defined(DEBUG_ESPUI)
    if (ESPUI.verbosity)
    {
        Serial.printf_P(PSTR("Deleting file: %s\n"), path);
    }
#endif

    bool didRemove = ESPUI.EspuiLittleFS.remove(path);
    if (didRemove)
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("File deleted"));
        }
#endif
    }
    else
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("Delete failed"));
        }
#endif
    }
}

void ESPUIClass::writeFile(const char* path, const char* data)
{
#if defined(DEBUG_ESPUI)
    if (ESPUI.verbosity)
    {
        Serial.printf_P(PSTR("Writing file: %s\n"), path);
    }
#endif

    File file = EspuiLittleFS.open(path, FILE_WRITING);
    if (!file)
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("Failed to open file for writing"));
        }
#endif

        return;
    }

#if defined(ESP32)
    if (file.print(data))
#else
    if (file.print(FPSTR(data)))
#endif // !defined(ESP32)

    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("File written"));
        }
    }
    else
    {
        if (ESPUI.verbosity)
        {
            Serial.println(F("Write failed"));
        }
#endif
    }
    file.close();
}

// end LITTLEFS functions

void ESPUIClass::prepareFileSystem(bool format)
{
    // this function should only be used once

#if defined(DEBUG_ESPUI)
    if (verbosity)
    {
        Serial.println(F("About to prepare filesystem..."));
    }
#endif

#if defined(ESP32)
    if (!EspuiLittleFS.begin(false)) // Test for an already formatted LittleFS by a mount failure
    {
        if (!EspuiLittleFS.begin(true)) // Attempt to format LittleFS
        {
#else
    if (!EspuiLittleFS.begin()) // Test for an already formatted LittleFS by a mount failure
    {
        if (EspuiLittleFS.format()) // Attempt to format LittleFS
        {
#endif // !defined(ESP32)

#if defined(DEBUG_ESPUI)
            if (verbosity)
            {
                Serial.println(F("LittleFS Format Failed"));
            }
#endif
            return;
        }
    }
    else if (format)
    {
        EspuiLittleFS.format();

#if defined(DEBUG_ESPUI)
        if (verbosity)
        {
            Serial.println(F("LittleFS Formatted"));
        }
#endif
    }

#if defined(DEBUG_ESPUI)
    if (verbosity)
    {
        listDir("/", 1);
        Serial.println(F("LittleFS Mount ESP32 Done"));
    }
#endif

    deleteFile("/index.htm");

    deleteFile("/css/style.css");
    deleteFile("/css/normalize.css");

    deleteFile("/js/zepto.min.js");
    deleteFile("/js/controls.js");
    deleteFile("/js/slider.js");
    deleteFile("/js/graph.js");
    deleteFile("/js/tabbedcontent.js");

#if defined(DEBUG_ESPUI)
    if (verbosity)
    {
        Serial.println(F("Cleanup done"));
    }
#endif

    // Now write
#ifdef ESP32
    writeFile("/index.htm", HTML_INDEX);
    EspuiLittleFS.mkdir("/css");
    writeFile("/css/style.css", CSS_STYLE);
    writeFile("/css/normalize.css", CSS_NORMALIZE);
    EspuiLittleFS.mkdir("/js");
    writeFile("/js/zepto.min.js", JS_ZEPTO);
    writeFile("/js/controls.js", JS_CONTROLS);
    writeFile("/js/slider.js", JS_SLIDER);
    writeFile("/js/graph.js", JS_GRAPH);

    writeFile("/js/tabbedcontent.js", JS_TABBEDCONTENT);

#else
    writeFile("/index.htm", HTML_INDEX);

    writeFile("/css/style.css", CSS_STYLE);
    writeFile("/css/normalize.css", CSS_NORMALIZE);

    writeFile("/js/zepto.min.js", JS_ZEPTO);
    writeFile("/js/controls.js", JS_CONTROLS);
    writeFile("/js/slider.js", JS_SLIDER);
    writeFile("/js/graph.js", JS_GRAPH);

    writeFile("/js/tabbedcontent.js", JS_TABBEDCONTENT);
#endif

#if defined(DEBUG_ESPUI)
    if (verbosity)
    {
        Serial.println(F("Done Initializing filesystem :-)"));
    }
#endif

#if defined(ESP32)

#if defined(DEBUG_ESPUI)
    if (verbosity)
    {
        listDir("/", 1);
    }
#endif

#endif

    EspuiLittleFS.end();
}

// Handle Websockets Communication
void ESPUIClass::onWsEvent(
    AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len)
{
    // Serial.println(String("ESPUIClass::OnWsEvent: type: ") + String(type));
    RemoveToBeDeletedControls();

    if (WS_EVT_DISCONNECT == type)
    {
#if defined(DEBUG_ESPUI)
        if (verbosity)
        {
            Serial.println(F("WS_EVT_DISCONNECT"));
        }
#endif

        if (MapOfClients.end() != MapOfClients.find(client->id()))
        {
            // Serial.println("Delete client.");
            delete MapOfClients[client->id()];
            MapOfClients.erase(client->id());
        }
    }
    else
    {
        if(type == WS_EVT_CONNECT)
        {
            ws->cleanupClients();
        }
        
        if (MapOfClients.end() == MapOfClients.find(client->id()))
        {
            // Serial.println("ESPUIClass::OnWsEvent:Create new client.");
            MapOfClients[client->id()] = new ESPUIclient(client);
        }

        if(MapOfClients[client->id()]->onWsEvent(type, arg, data, len))
        {
            // Serial.println("ESPUIClass::OnWsEvent:notify the clients that they need to be updated.");
            NotifyClients(ESPUIclient::UpdateNeeded);
        }
    }

    return;
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label)
{
    return addControl(type, label, String(""));
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label, const String& value)
{
    return addControl(type, label, value, ControlColor::Turquoise);
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label, const String& value, ControlColor color)
{
    return addControl(type, label, value, color, Control::noParent);
}

uint16_t ESPUIClass::addControl(
    ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl)
{
    return addControl(type, label, value, color, parentControl, new Control(type, label, nullptr, value, color, true, parentControl));
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label, const String& value, ControlColor color,
    uint16_t parentControl, std::function<void(Control*, int)> callback)
{
    uint16_t id = addControl(type, label, value, color, parentControl);
    // set the original style callback
    getControl(id)->callback = callback;
    return id;
}

uint16_t ESPUIClass::addControl(
    ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl, Control* control)
{
#ifdef ESP32
    xSemaphoreTake(ControlsSemaphore, portMAX_DELAY);
#endif // def ESP32

    if (controls == nullptr)
    {
        controls = control;
    }
    else
    {
        Control* iterator = controls;

        while (iterator->next != nullptr)
        {
            iterator = iterator->next;
        }

        iterator->next = control;
    }

    controlCount++;

#ifdef ESP32
    xSemaphoreGive(ControlsSemaphore);
#endif // def ESP32

    NotifyClients(ClientUpdateType_t::RebuildNeeded);

    return control->id;
}

bool ESPUIClass::removeControl(uint16_t id, bool force_rebuild_ui)
{
    bool Response = false;

    Control* control = getControl(id);
    if (control)
    {
        Response = true;
        control->DeleteControl();
        controlCount--;

        if (force_rebuild_ui)
        {
            jsonReload();
        }
        else
        {
            NotifyClients(ClientUpdateType_t::RebuildNeeded);
        }
    }
#ifdef DEBUG_ESPUI
    else
    {
        // Serial.println(String("Could not Remove Control ") + String(id));
    }
#endif // def DEBUG_ESPUI

    return Response;
}

void ESPUIClass::RemoveToBeDeletedControls()
{
#ifdef ESP32
    xSemaphoreTake(ControlsSemaphore, portMAX_DELAY);
#endif // def ESP32

    Control* PreviousControl = nullptr;
    Control* CurrentControl = controls;

    while (nullptr != CurrentControl)
    {
        Control* NextControl = CurrentControl->next;
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

uint16_t ESPUIClass::label(const char* label, ControlColor color, const String& value)
{
    return addControl(ControlType::Label, label, value, color);
}

uint16_t ESPUIClass::graph(const char* label, ControlColor color)
{
    return addControl(ControlType::Graph, label, "", color);
}

uint16_t ESPUIClass::slider(
    const char* label, std::function<void(Control*, int)> callback, ControlColor color, int value, int min, int max)
{
    uint16_t sliderId
        = addControl(ControlType::Slider, label, String(value), color, Control::noParent, callback);
    addControl(ControlType::Min, label, String(min), ControlColor::None, sliderId);
    addControl(ControlType::Max, label, String(max), ControlColor::None, sliderId);
    return sliderId;
}

uint16_t ESPUIClass::button(const char* label, std::function<void(Control*, int)> callback, ControlColor color, const String& value)
{
    return addControl(ControlType::Button, label, value, color, Control::noParent, callback);
}

uint16_t ESPUIClass::switcher(const char* label, std::function<void(Control*, int)> callback, ControlColor color, bool startState)
{
    return addControl(ControlType::Switcher, label, startState ? "1" : "0", color, Control::noParent, callback);
}

uint16_t ESPUIClass::pad(const char* label, std::function<void(Control*, int)> callback, ControlColor color)
{
    return addControl(ControlType::Pad, label, "", color, Control::noParent, callback);
}

uint16_t ESPUIClass::padWithCenter(const char* label, std::function<void(Control*, int)> callback, ControlColor color)
{
    return addControl(ControlType::PadWithCenter, label, "", color, Control::noParent, callback);
}

uint16_t ESPUIClass::number(
    const char* label, std::function<void(Control*, int)> callback, ControlColor color, int number, int min, int max)
{
    uint16_t numberId = addControl(ControlType::Number, label, String(number), color, Control::noParent, callback);
    addControl(ControlType::Min, label, String(min), ControlColor::None, numberId);
    addControl(ControlType::Max, label, String(max), ControlColor::None, numberId);
    return numberId;
}

uint16_t ESPUIClass::gauge(const char* label, ControlColor color, int number, int min, int max)
{
    uint16_t numberId = addControl(ControlType::Gauge, label, String(number), color, Control::noParent);
    addControl(ControlType::Min, label, String(min), ControlColor::None, numberId);
    addControl(ControlType::Max, label, String(max), ControlColor::None, numberId);
    return numberId;
}

uint16_t ESPUIClass::separator(const char* label)
{
    return addControl(ControlType::Separator, label, "", ControlColor::Alizarin);
}

uint16_t ESPUIClass::fileDisplay(const char* label, ControlColor color, String filename)
{
    return addControl(ControlType::FileDisplay, label, filename, color, Control::noParent);
}

uint16_t ESPUIClass::accelerometer(const char* label, std::function<void(Control*, int)> callback, ControlColor color)
{
    return addControl(ControlType::Accel, label, "", color, Control::noParent, callback);
}

uint16_t ESPUIClass::text(const char* label, std::function<void(Control*, int)> callback, ControlColor color, const String& value)
{
    return addControl(ControlType::Text, label, value, color, Control::noParent, callback);
}

Control* ESPUIClass::getControl(uint16_t id)
{
#ifdef ESP32
    xSemaphoreTake(ControlsSemaphore, portMAX_DELAY);
    Control* Response = getControlNoLock(id);
    xSemaphoreGive(ControlsSemaphore);
    return Response;
#else
    return getControlNoLock(id);
#endif // !def ESP32
}

// WARNING: Anytime you walk the chain of controllers, the protection semaphore
//          MUST be locked. This function assumes that the semaphore is locked
//          at the time it is called. Make sure YOU locked it :)
Control* ESPUIClass::getControlNoLock(uint16_t id)
{
    Control* Response = nullptr;
    Control* control = controls;

    while (nullptr != control)
    {
        if (control->id == id)
        {
            if (!control->ToBeDeleted())
            {
                Response = control;
            }
            break;
        }
        control = control->next;
    }

    return Response;
}

void ESPUIClass::updateControl(Control* control, int)
{
    if (!control)
    {
        return;
    }
    // tell the control it has been updated
    control->SetControlChangedId(ESPUI.GetNextControlChangeId());
    NotifyClients(ClientUpdateType_t::UpdateNeeded);
}

uint32_t ESPUIClass::GetNextControlChangeId()
{
    if(uint32_t(-1) == ControlChangeID)
    {
        // force a reload which resets the counters
        jsonReload();
    }
    return ++ControlChangeID;
}

void ESPUIClass::setPanelStyle(uint16_t id, const String& style, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->panelStyle = style;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setElementStyle(uint16_t id, const String& style, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->elementStyle = style;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setInputType(uint16_t id, const String& type, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->inputType = type;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setPanelWide(uint16_t id, bool wide)
{
    Control* control = getControl(id);
    if (control)
    {
        control->wide = wide;
    }
}

void ESPUIClass::setEnabled(uint16_t id, bool enabled, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        // Serial.println(String("CreateAllowed: id: ") + String(clientId) + " State: " + String(enabled));
        control->enabled = enabled;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setVertical(uint16_t id, bool vert)
{
    Control* control = getControl(id);
    if (control)
    {
        control->vertical = vert;
    }
}

void ESPUIClass::updateControl(uint16_t id, int clientId)
{
    Control* control = getControl(id);

    if (!control)
    {
#if defined(DEBUG_ESPUI)
        if (verbosity)
        {
            Serial.printf_P(PSTR("Error: Update Control: There is no control with ID %d\n"), id);
        }
#endif
        return;
    }

    updateControl(control, clientId);
}

void ESPUIClass::updateControlValue(Control* control, const String& value, int clientId)
{
    if (!control)
    {
        return;
    }

    control->value = value;
    updateControl(control, clientId);
}

void ESPUIClass::updateControlValue(uint16_t id, const String& value, int clientId)
{
    Control* control = getControl(id);

    if (!control)
    {
#if defined(DEBUG_ESPUI)
        if (verbosity)
        {
            Serial.printf_P(PSTR("Error: updateControlValue Control: There is no control with ID %d\n"), id);
        }
#endif
        return;
    }

    updateControlValue(control, value, clientId);
}

void ESPUIClass::updateControlLabel(uint16_t id, const char* value, int clientId)
{
    updateControlLabel(getControl(id), value, clientId);
}

void ESPUIClass::updateControlLabel(Control* control, const char* value, int clientId)
{
    if (!control)
    {
#if defined(DEBUG_ESPUI)
        if (verbosity)
        {
            Serial.printf_P(PSTR("Error: updateControlLabel Control: There is no control with the requested ID \n"));
        }
#endif
        return;
    }
    control->label = value;
    updateControl(control, clientId);
}

void ESPUIClass::updateVisibility(uint16_t id, bool visibility, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->visible = visibility;
        updateControl(control, clientId);
    }
}

void ESPUIClass::print(uint16_t id, const String& value)
{
    updateControlValue(id, value);
}

void ESPUIClass::updateLabel(uint16_t id, const String& value)
{
    updateControlValue(id, value);
}

void ESPUIClass::updateButton(uint16_t id, const String& value)
{
    updateControlValue(id, value);
}

void ESPUIClass::updateSlider(uint16_t id, int nValue, int clientId)
{
    updateControlValue(id, String(nValue), clientId);
}

void ESPUIClass::updateSwitcher(uint16_t id, bool nValue, int clientId)
{
    updateControlValue(id, String(nValue ? "1" : "0"), clientId);
}

void ESPUIClass::updateNumber(uint16_t id, int number, int clientId)
{
    updateControlValue(id, String(number), clientId);
}

void ESPUIClass::updateText(uint16_t id, const String& text, int clientId)
{
    updateControlValue(id, text, clientId);
}

void ESPUIClass::updateSelect(uint16_t id, const String& text, int clientId)
{
    updateControlValue(id, text, clientId);
}

void ESPUIClass::updateGauge(uint16_t id, int number, int clientId)
{
    updateControlValue(id, String(number), clientId);
}

void ESPUIClass::updateTime(uint16_t id, int clientId)
{
    updateControl(id, clientId);
}

void ESPUIClass::clearGraph(uint16_t id, int clientId)
{
    do // once
    {
        Control* control = getControl(id);
        if (!control)
        {
            break;
        }

        AllocateJsonDocument(document, jsonUpdateDocumentSize);
        JsonObject root = document.to<JsonObject>();

        root[F("type")] = (int)ControlType::Graph + UpdateOffset;
        root[F("value")] = 0;
        root[F("id")] = control->id;

        SendJsonDocToWebSocket(document, clientId);

    } while (false);
}

void ESPUIClass::addGraphPoint(uint16_t id, int nValue, int clientId)
{
    do // once
    {
        Control* control = getControl(id);
        if (!control)
        {
            break;
        }

        AllocateJsonDocument(document, jsonUpdateDocumentSize);
        JsonObject root = document.to<JsonObject>();

        root[F("type")] = (int)ControlType::GraphPoint;
        root[F("value")] = nValue;
        root[F("id")] = control->id;

        SendJsonDocToWebSocket(document, clientId);

    } while (false);
}

bool ESPUIClass::SendJsonDocToWebSocket(ArduinoJson::JsonDocument& document, uint16_t clientId)
{
    bool Response = false;

    if (0 > clientId)
    {
        if (MapOfClients.end() != MapOfClients.find(clientId))
        {
            Response = MapOfClients[clientId]->SendJsonDocToWebSocket(document);
        }
    }
    else
    {
        for (auto CurrentClient : MapOfClients)
        {
            Response |= CurrentClient.second->SendJsonDocToWebSocket(document);
        }
    }

    return Response;
}

void ESPUIClass::jsonDom(uint16_t, AsyncWebSocketClient*, bool)
{
    NotifyClients(ClientUpdateType_t::RebuildNeeded);
}

// Tell all of the clients that they need to ask for an upload of the control data.
void ESPUIClass::NotifyClients(ClientUpdateType_t newState)
{
    for (auto& CurrentClient : MapOfClients)
    {
        CurrentClient.second->NotifyClient(newState);
    }
}

void ESPUIClass::jsonReload()
{
    for (auto& CurrentClient : MapOfClients)
    {
        // Serial.println("Requesting Reload");
        CurrentClient.second->NotifyClient(ClientUpdateType_t::ReloadNeeded);
    }
}

void ESPUIClass::beginSPIFFS(const char* _title, const char* username, const char* password, uint16_t port)
{
    // Backwards compatibility wrapper
    beginLITTLEFS(_title, username, password, port);
}

void ESPUIClass::beginLITTLEFS(const char* _title, const char* username, const char* password, uint16_t port)
{
    ui_title = _title;
    basicAuthUsername = username;
    basicAuthPassword = password;

    if (username == nullptr && password == nullptr)
    {
        basicAuth = false;
    }
    else
    {
        basicAuth = true;
    }

    server = new AsyncWebServer(port);
    ws = new AsyncWebSocket("/ws");

    bool fsBegin = EspuiLittleFS.begin();
    if (!fsBegin)
    {
#if defined(DEBUG_ESPUI)
        if (verbosity)
        {
            Serial.println(F("LITTLEFS Mount Failed, PLEASE CHECK THE README ON HOW TO "
                             "PREPARE YOUR ESP!!!!!!!"));
        }
#endif

        return;
    }

#if defined(DEBUG_ESPUI)
    if (verbosity)
    {
        listDir("/", 1);
    }
#endif

    bool indexExists = EspuiLittleFS.exists("/index.htm");
    if (!indexExists)
    {
#if defined(DEBUG_ESPUI)
        if (verbosity)
        {
            Serial.println(F("Please read the README!!!!!!!, Make sure to "
                             "prepareFileSystem() once in an empty sketch"));
        }
#endif

        return;
    }

    ws->onEvent([](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data,
                    size_t len) { ESPUI.onWsEvent(server, client, type, arg, data, len); });
    server->addHandler(ws);

    if (basicAuth)
    {
        if (WS_AUTHENTICATION)
        {
            ws->setAuthentication(basicAuthUsername, basicAuthPassword);
        }
        server->serveStatic("/", EspuiLittleFS, "/").setDefaultFile("index.htm").setAuthentication(username, password);
    }
    else
    {
        server->serveStatic("/", EspuiLittleFS, "/").setDefaultFile("index.htm");
    }

    // Heap for general Servertest
    server->on("/heap", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        request->send(200, "text/plain", heapInfo(F("In LITTLEFS mode")));
    });

    server->onNotFound([this](AsyncWebServerRequest* request) {
        if (captivePortal)
        {
            request->redirect("/");
        }
        else
        {
            request->send(404);
        }
    });

    server->begin();

#if defined(DEBUG_ESPUI)
    if (verbosity)
    {
        Serial.println(F("UI Initialized"));
    }
#endif
}

void ESPUIClass::begin(const char* _title, const char* username, const char* password, uint16_t port)
{
    basicAuthUsername = username;
    basicAuthPassword = password;

    if (username != nullptr && password != nullptr)
    {
        basicAuth = true;
    }
    else
    {
        basicAuth = false;
    }

    ui_title = _title;

    server = new AsyncWebServer(port);
    ws = new AsyncWebSocket("/ws");

    ws->onEvent([](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data,
                    size_t len) { ESPUI.onWsEvent(server, client, type, arg, data, len); });

    server->addHandler(ws);

    if (basicAuth && WS_AUTHENTICATION)
        ws->setAuthentication(username, password);

    server->on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        AsyncWebServerResponse* response = request->beginResponse_P(200, "text/html", HTML_INDEX);
        request->send(response);
    });

    // Javascript files

    server->on("/js/zepto.min.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        AsyncWebServerResponse* response
            = request->beginResponse_P(200, "application/javascript", JS_ZEPTO_GZIP, sizeof(JS_ZEPTO_GZIP));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/js/controls.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        AsyncWebServerResponse* response
            = request->beginResponse_P(200, "application/javascript", JS_CONTROLS_GZIP, sizeof(JS_CONTROLS_GZIP));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/js/slider.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        AsyncWebServerResponse* response
            = request->beginResponse_P(200, "application/javascript", JS_SLIDER_GZIP, sizeof(JS_SLIDER_GZIP));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/js/graph.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        AsyncWebServerResponse* response
            = request->beginResponse_P(200, "application/javascript", JS_GRAPH_GZIP, sizeof(JS_GRAPH_GZIP));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/js/tabbedcontent.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        AsyncWebServerResponse* response = request->beginResponse_P(
            200, "application/javascript", JS_TABBEDCONTENT_GZIP, sizeof(JS_TABBEDCONTENT_GZIP));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // Stylesheets

    server->on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        AsyncWebServerResponse* response
            = request->beginResponse_P(200, "text/css", CSS_STYLE_GZIP, sizeof(CSS_STYLE_GZIP));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/css/normalize.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        AsyncWebServerResponse* response
            = request->beginResponse_P(200, "text/css", CSS_NORMALIZE_GZIP, sizeof(CSS_NORMALIZE_GZIP));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // Heap for general Servertest
    server->on("/heap", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        request->send(200, "text/plain", heapInfo(F("In Memorymode")));
    });

    server->onNotFound([this](AsyncWebServerRequest* request) {
        if (captivePortal)
        {
            AsyncResponseStream *response = request->beginResponseStream("text/html");
            String responseText;
            responseText.reserve(1024);
            responseText += F("<!DOCTYPE html><html><head><title>Captive Portal</title></head><body>");
            responseText += ("<p>If site does not re-direct click here <a href='http://" +  WiFi.softAPIP().toString() + "'>this link</a></p>");
            responseText += ("</body></html><head><meta http-equiv=\"Refresh\" content=\"0; URL='http://" +  WiFi.softAPIP().toString() + "'\" /></head>");
            response->write(responseText.c_str(), responseText.length());
            request->send(response);
        }
        else
        {
            request->send(404);
        }
        yield();
    });

    server->begin();

#if defined(DEBUG_ESPUI)
    if (verbosity)
    {
        Serial.println(F("UI Initialized"));
    }
#endif
}

void ESPUIClass::setVerbosity(Verbosity v)
{
    verbosity = v;
}

ESPUIClass ESPUI;
