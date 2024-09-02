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
#include "ESPUIcontrolMgr.h"

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
    ESPUIcontrolMgr.RemoveToBeDeletedControls();

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

Control::ControlId_t ESPUIClass::addControl(Control::Type type, const char* label)
{
    return addControl(type, label, emptyString);
}

Control::ControlId_t ESPUIClass::addControl(Control::Type type, const char* label, const String& value)
{
    return addControl(type, label, value, Control::Color::Turquoise);
}

Control::ControlId_t ESPUIClass::addControl(Control::Type type, const char* label, const String& value, Control::Color color)
{
    return addControl(type, label, value, color, Control::noParent);
}

Control::ControlId_t ESPUIClass::addControl(Control::Type type, 
const char* label, const String& value, Control::Color color, Control::ControlId_t parentControlId)
{
    return addControl(type, label, value, color, parentControlId, (std::function<void(Control*, int)>)nullptr);
}

Control::ControlId_t ESPUIClass::addControl(Control::Type type,
    const char* label, const String& value, Control::Color color,
    Control::ControlId_t parentControlId, std::function<void(Control*, int)> callback)
{
    Control::ControlId_t id = ESPUIcontrolMgr.addControl(type, label, value, color, parentControlId, true, callback);
    NotifyClients(ClientUpdateType_t::RebuildNeeded);
    return id;
}

bool ESPUIClass::removeControl(Control::ControlId_t id, bool force_rebuild_ui)
{
    bool Response = ESPUIcontrolMgr.removeControl(id);
    if (force_rebuild_ui)
    {
        ESPUI.jsonReload();
    }
    else
    {
        ESPUI.NotifyClients(ClientUpdateType_t::RebuildNeeded);
    }
    return Response;
} // removeControl

Control::ControlId_t ESPUIClass::label(const char* label, Control::Color color, const String& value)
{
    return addControl(Control::Type::Label, label, value, color);
}

Control::ControlId_t ESPUIClass::graph(const char* label, Control::Color color)
{
    return addControl(Control::Type::Graph, label, "", color);
}

Control::ControlId_t ESPUIClass::slider(
    const char* label, std::function<void(Control*, int)> callback, Control::Color color, int value, int min, int max)
{
    Control::ControlId_t sliderId
        = addControl(Control::Type::Slider, label, String(value), color, Control::noParent, callback);
    addControl(Control::Type::Min, label, String(min), Control::Color::None, sliderId);
    addControl(Control::Type::Max, label, String(max), Control::Color::None, sliderId);
    return sliderId;
}

Control::ControlId_t ESPUIClass::button(const char* label, std::function<void(Control*, int)> callback, Control::Color color, const String& value)
{
    return addControl(Control::Type::Button, label, value, color, Control::noParent, callback);
}

Control::ControlId_t ESPUIClass::switcher(const char* label, std::function<void(Control*, int)> callback, Control::Color color, bool startState)
{
    return addControl(Control::Type::Switcher, label, startState ? "1" : "0", color, Control::noParent, callback);
}

Control::ControlId_t ESPUIClass::pad(const char* label, std::function<void(Control*, int)> callback, Control::Color color)
{
    return addControl(Control::Type::Pad, label, "", color, Control::noParent, callback);
}

Control::ControlId_t ESPUIClass::padWithCenter(const char* label, std::function<void(Control*, int)> callback, Control::Color color)
{
    return addControl(Control::Type::PadWithCenter, label, "", color, Control::noParent, callback);
}

Control::ControlId_t ESPUIClass::number(
    const char* label, std::function<void(Control*, int)> callback, Control::Color color, int number, int min, int max)
{
    Control::ControlId_t numberId = addControl(Control::Type::Number, label, String(number), color, Control::noParent, callback);
    addControl(Control::Type::Min, label, String(min), Control::Color::None, numberId);
    addControl(Control::Type::Max, label, String(max), Control::Color::None, numberId);
    return numberId;
}

Control::ControlId_t ESPUIClass::gauge(const char* label, Control::Color color, int number, int min, int max)
{
    Control::ControlId_t numberId = addControl(Control::Type::Gauge, label, String(number), color, Control::noParent);
    addControl(Control::Type::Min, label, String(min), Control::Color::None, numberId);
    addControl(Control::Type::Max, label, String(max), Control::Color::None, numberId);
    return numberId;
}

Control::ControlId_t ESPUIClass::separator(const char* label)
{
    return addControl(Control::Type::Separator, label, "", Control::Color::Alizarin, Control::noParent, nullptr);
}

Control::ControlId_t ESPUIClass::fileDisplay(const char* label, Control::Color color, String filename)
{
    return addControl(Control::Type::FileDisplay, label, filename, color, Control::noParent);
}

Control::ControlId_t ESPUIClass::accelerometer(const char* label, std::function<void(Control*, int)> callback, Control::Color color)
{
    return addControl(Control::Type::Accel, label, "", color, Control::noParent, callback);
}

Control::ControlId_t ESPUIClass::text(const char* label, std::function<void(Control*, int)> callback, Control::Color color, const String& value)
{
    return addControl(Control::Type::Text, label, value, color, Control::noParent, callback);
}

Control* ESPUIClass::getControl(Control::ControlId_t id) {return ESPUIcontrolMgr.getControl(id);}
Control* ESPUIClass::getControlNoLock(Control::ControlId_t id) {return ESPUIcontrolMgr.getControlNoLock(id);}

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

void ESPUIClass::setPanelStyle(Control::ControlId_t id, const String& style, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->panelStyle = style;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setElementStyle(Control::ControlId_t id, const String& style, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->elementStyle = style;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setInputType(Control::ControlId_t id, const String& type, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->inputType = type;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setPanelWide(Control::ControlId_t id, bool wide)
{
    Control* control = getControl(id);
    if (control)
    {
        control->wide = wide;
    }
}

void ESPUIClass::setEnabled(Control::ControlId_t id, bool enabled, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        // Serial.println(String("CreateAllowed: id: ") + String(clientId) + " State: " + String(enabled));
        control->enabled = enabled;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setVertical(Control::ControlId_t id, bool vert)
{
    Control* control = getControl(id);
    if (control)
    {
        control->vertical = vert;
    }
}

void ESPUIClass::updateControl(Control::ControlId_t id, int clientId)
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

void ESPUIClass::updateControlValue(Control::ControlId_t id, const String& value, int clientId)
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

void ESPUIClass::updateControlLabel(Control::ControlId_t id, const char* value, int clientId)
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

void ESPUIClass::updateVisibility(Control::ControlId_t id, bool visibility, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->visible = visibility;
        updateControl(control, clientId);
    }
}

void ESPUIClass::print(Control::ControlId_t id, const String& value)
{
    updateControlValue(id, value);
}

void ESPUIClass::updateLabel(Control::ControlId_t id, const String& value)
{
    updateControlValue(id, value);
}

void ESPUIClass::updateButton(Control::ControlId_t id, const String& value)
{
    updateControlValue(id, value);
}

void ESPUIClass::updateSlider(Control::ControlId_t id, int nValue, int clientId)
{
    updateControlValue(id, String(nValue), clientId);
}

void ESPUIClass::updateSwitcher(Control::ControlId_t id, bool nValue, int clientId)
{
    updateControlValue(id, String(nValue ? "1" : "0"), clientId);
}

void ESPUIClass::updateNumber(Control::ControlId_t id, int number, int clientId)
{
    updateControlValue(id, String(number), clientId);
}

void ESPUIClass::updateText(Control::ControlId_t id, const String& text, int clientId)
{
    updateControlValue(id, text, clientId);
}

void ESPUIClass::updateSelect(Control::ControlId_t id, const String& text, int clientId)
{
    updateControlValue(id, text, clientId);
}

void ESPUIClass::updateGauge(Control::ControlId_t id, int number, int clientId)
{
    updateControlValue(id, String(number), clientId);
}

void ESPUIClass::updateTime(Control::ControlId_t id, int clientId)
{
    updateControl(id, clientId);
}

void ESPUIClass::clearGraph(Control::ControlId_t id, int clientId)
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

        root[F("type")] = (int)Control::Type::Graph + Control::Type::UpdateOffset;
        root[F("value")] = 0;
        root[F("id")] = control->GetId();

        SendJsonDocToWebSocket(document, clientId);

    } while (false);
}

void ESPUIClass::addGraphPoint(Control::ControlId_t id, int nValue, int clientId)
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

        root[F("type")] = (int)Control::Type::GraphPoint;
        root[F("value")] = nValue;
        root[F("id")] = control->GetId();

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
