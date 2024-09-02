#pragma once

// comment out to turn off debug output
// #define DEBUG_ESPUI true
#define WS_AUTHENTICATION false

#include <Arduino.h>

#include <ArduinoJson.h>
#if ARDUINOJSON_VERSION_MAJOR > 6
    #define AllocateJsonDocument(name, size)    JsonDocument name
    #define AllocateJsonArray(doc, name)        doc[name].to<JsonArray>()
    #define AllocateJsonObject(doc)             doc.add<JsonObject>()
    #define AllocateNamedJsonObject(t, s, n)    t[n] = s
#else
    #define AllocateJsonDocument(name, size)    DynamicJsonDocument name(size)
    #define AllocateJsonArray(doc, name)        doc.createNestedArray(name)
    #define AllocateJsonObject(doc)             doc.createNestedObject()
    #define AllocateNamedJsonObject(t, s, n)    t = s.createNestedObject(n)
#endif

#include <stdlib_noniso.h>
#ifdef ESP32
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		#include <LittleFS.h>
	#else
		#include <LITTLEFS.h>
	#endif
#else
	#include <LittleFS.h>
#endif
#include <map>
#include <ESPAsyncWebServer.h>

#include "ESPUIcontrol.h"
#include "ESPUIclient.h"

#if defined(ESP32)
#include <AsyncTCP.h>
#include "WiFi.h"

#else

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <Hash.h>

#endif

#include <ESPUIcontrolMgr.h>

#define FILE_WRITING "w"

// Message Types (and control types)

enum MessageTypes : uint8_t
{
    InitialGui = 200,
    Reload = 201,
    ExtendGUI = 210,
    UpdateGui = 220,
    ExtendedUpdateGui = 230,
};

#define UI_INITIAL_GUI  MessageTypes::InitialGui
#define UI_EXTEND_GUI   MessageTypes::ExtendGUI
#define UI_RELOAD       MessageTypes::Reload

// Values
#define B_DOWN -1
#define B_UP 1

#define P_LEFT_DOWN -2
#define P_LEFT_UP 2
#define P_RIGHT_DOWN -3
#define P_RIGHT_UP 3
#define P_FOR_DOWN -4
#define P_FOR_UP 4
#define P_BACK_DOWN -5
#define P_BACK_UP 5
#define P_CENTER_DOWN -6
#define P_CENTER_UP 6

#define S_ACTIVE -7
#define S_INACTIVE 7

#define SL_VALUE 8
#define N_VALUE 9
#define T_VALUE 10
#define S_VALUE 11
#define TM_VALUE 12

enum Verbosity : uint8_t
{
    Quiet = 0,
    Verbose,
    VerboseJSON
};

class ESPUIClass
{
public:
    ESPUIClass()
    {
	}
    unsigned int jsonUpdateDocumentSize = 2000;
#ifdef ESP8266
    unsigned int jsonInitialDocumentSize = 2000;
    unsigned int jsonChunkNumberMax = 5;
#else
    unsigned int jsonInitialDocumentSize = 8000;
    unsigned int jsonChunkNumberMax = 0;
#endif
    bool sliderContinuous = false;
    void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);
	bool captivePortal = true;

    void setVerbosity(Verbosity verbosity);
    void begin(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in Memorymode
    void beginSPIFFS(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in LITTLEFS mode (DEPRECATED, use beginLITTLEFS)
    void beginLITTLEFS(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in LITTLEFS mode

    void prepareFileSystem(bool format = true); // Initially preps the filesystem and loads a lot of
                              // stuff into LITTLEFS
    void list(); // Lists LITTLEFS directory
    void writeFile(const char* path, const char* data);

    Control::ControlId_t addControl(Control::Type type, const char* label);
    Control::ControlId_t addControl(Control::Type type, const char* label, const String& value);
    Control::ControlId_t addControl(Control::Type type, const char* label, const String& value, Control::Color color);
    Control::ControlId_t addControl(Control::Type type, const char* label, const String& value, Control::Color color, Control::ControlId_t parentControlId);
    Control::ControlId_t addControl(Control::Type type, const char* label, const String& value, Control::Color color, Control::ControlId_t parentControlId, std::function<void(Control*, int)> callback);

    bool removeControl(Control::ControlId_t id, bool force_rebuild_ui = false);

    // create Elements
    // Create Event Button
    Control::ControlId_t button(const char* label, std::function<void(Control*, int)> callback, Control::Color color, const String& value = "");
    Control::ControlId_t switcher(const char* label, std::function<void(Control*, int)> callback, Control::Color color, bool startState = false); // Create Toggle Button
    Control::ControlId_t pad(const char* label, std::function<void(Control*, int)> callback, Control::Color color); // Create Pad Control
    Control::ControlId_t padWithCenter(const char* label, std::function<void(Control*, int)> callback, Control::Color color); // Create Pad Control with Centerbutton
    Control::ControlId_t slider(const char* label, std::function<void(Control*, int)> callback, Control::Color color, int value, int min = 0, int max = 100); // Create Slider Control
    Control::ControlId_t number(const char* label, std::function<void(Control*, int)> callback, Control::Color color, int value, int min = 0, int max = 100); // Create a Number Input Control
    Control::ControlId_t text(const char* label, std::function<void(Control*, int)> callback, Control::Color color, const String& value = ""); // Create a Text Input Control

    // Output only
    Control::ControlId_t label(const char* label, Control::Color color,
        const String& value = ""); // Create Label
    Control::ControlId_t graph(const char* label, Control::Color color); // Create Graph display
    Control::ControlId_t gauge(const char* label, Control::Color color, int value, int min = 0,
        int max = 100); // Create Gauge display
    Control::ControlId_t separator(const char* label); //Create separator
    Control::ControlId_t fileDisplay(const char* label, Control::Color color, String filename);

    // Input only
    Control::ControlId_t accelerometer(const char* label, std::function<void(Control*, int)> callback, Control::Color color);

    // Update Elements

    Control* getControl(Control::ControlId_t id);
    Control* getControlNoLock(Control::ControlId_t id);

    // Update Elements
    void updateControlValue(Control::ControlId_t id, const String& value, int clientId = -1);
    void updateControlValue(Control* control, const String& value, int clientId = -1);

    void updateControlLabel(Control::ControlId_t control, const char * value, int clientId = -1);
    void updateControlLabel(Control* control, const char * value, int clientId = -1);

    void updateControl(Control::ControlId_t id, int clientId = -1);
    void updateControl(Control* control, int clientId = -1);

    void print(Control::ControlId_t id, const String& value);
    void updateLabel(Control::ControlId_t id, const String& value);
    void updateButton(Control::ControlId_t id, const String& value);
    void updateSwitcher(Control::ControlId_t id, bool nValue, int clientId = -1);
    void updateSlider(Control::ControlId_t id, int nValue, int clientId = -1);
    void updateNumber(Control::ControlId_t id, int nValue, int clientId = -1);
    void updateText(Control::ControlId_t id, const String& nValue, int clientId = -1);
    void updateSelect(Control::ControlId_t id, const String& nValue, int clientId = -1);
    void updateGauge(Control::ControlId_t id, int number, int clientId);
    void updateTime(Control::ControlId_t id, int clientId = -1);

    void clearGraph(Control::ControlId_t id, int clientId = -1);
    void addGraphPoint(Control::ControlId_t id, int nValue, int clientId = -1);

    void setPanelStyle(Control::ControlId_t id, const String& style, int clientId = -1);
    void setElementStyle(Control::ControlId_t id, const String& style, int clientId = -1);
    void setInputType(Control::ControlId_t id, const String& type, int clientId = -1);

    void setPanelWide(Control::ControlId_t id, bool wide);
    void setVertical(Control::ControlId_t id, bool vert = true);
    void setEnabled(Control::ControlId_t id, bool enabled = true, int clientId = -1);

    void updateVisibility(Control::ControlId_t id, bool visibility, int clientId = -1);

    // Variables
    const char* ui_title = "ESPUI"; // Store UI Title and Header Name
    void jsonReload();
    void jsonDom(uint16_t startidx, AsyncWebSocketClient* client = nullptr, bool Updating = false);

    Verbosity verbosity = Verbosity::Quiet;
    uint32_t  GetNextControlChangeId();
    // emulate former extended callback API by using an intermediate lambda (no deprecation)
    Control::ControlId_t addControl(Control::Type type, const char* label, const String& value, Control::Color color, Control::ControlId_t parentControl, std::function<void(Control*, int, void*)> callback, void* userData)
    {
        return addControl(type, label, value, color, parentControl, [callback, userData](Control* sender, int type){ callback(sender, type, userData); });
    }
    Control::ControlId_t button(const char* label, std::function<void(Control*, int, void*)> callback, Control::Color color, const String& value, void* userData)
    {
        return button(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color, value);
    }
    Control::ControlId_t switcher(const char* label, std::function<void(Control*, int, void*)> callback, Control::Color color, bool startState, void* userData)
    {
        return switcher(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color, startState);
    }
    Control::ControlId_t pad(const char* label, std::function<void(Control*, int, void*)> callback, Control::Color color, void* userData)
    {
        return pad(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color);
    }
    Control::ControlId_t padWithCenter(const char* label, std::function<void(Control*, int, void*)> callback, Control::Color color, void* userData)
    {
        return padWithCenter(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color);
    }
    Control::ControlId_t slider(const char* label, std::function<void(Control*, int, void*)> callback, Control::Color color, int value, int min, int max, void* userData)
    {
        return slider(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color, value, min, max);
    }
    Control::ControlId_t number(const char* label, std::function<void(Control*, int, void*)> callback, Control::Color color, int value, int min, int max, void* userData)
    {
        return number(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color, value, min, max);
    }
    Control::ControlId_t text(const char* label, std::function<void(Control*, int, void*)> callback, Control::Color color, const String& value, void* userData)
    {
        return text(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); } , color, value);
    }
    Control::ControlId_t accelerometer(const char* label, std::function<void(Control*, int, void*)> callback, Control::Color color, void* userData)
    {
        return accelerometer(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color);
    }

    AsyncWebServer* WebServer() {return server;}
    AsyncWebSocket* WebSocket() {return ws;}

#if defined(ESP32)
#   if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
        fs::LittleFSFS & EspuiLittleFS = LittleFS;
    #else
        fs::LITTLEFSFS & EspuiLittleFS = LITTLEFS;
#   endif
#else
    fs::FS & EspuiLittleFS = LittleFS;
#endif

protected:
    friend class ESPUIclient;
    friend class ESPUIcontrol;

    AsyncWebServer* server;
    AsyncWebSocket* ws;

    const char* basicAuthUsername = nullptr;
    const char* basicAuthPassword = nullptr;
    bool basicAuth = true;

    Control::ControlId_t addControl(Control::Type type, const char* label, const String& value, Control::Color color, Control::ControlId_t parentControl, Control* control);

#define ClientUpdateType_t ESPUIclient::ClientUpdateType_t
    void NotifyClients(ClientUpdateType_t newState);
    void NotifyClient(uint32_t WsClientId, ClientUpdateType_t newState);

    bool SendJsonDocToWebSocket(ArduinoJson::JsonDocument& document, uint16_t clientId);

    std::map<uint32_t, ESPUIclient*> MapOfClients;

    uint32_t    ControlChangeID = 0;
};

extern ESPUIClass ESPUI;
