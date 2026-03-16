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

// Platform detection for ESP32, ESP8266, RP2040, RP2350
#if defined(ESP32)
    #define ESPUI_PLATFORM "ESP32"
    #define ESPUI_USING_ASYNC 1
    #define ESPUI_USING_FREERTOS 1
    #if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
        #include <LittleFS.h>
    #else
        #include <LITTLEFS.h>
    #endif
#elif defined(ESP8266)
    #define ESPUI_PLATFORM "ESP8266"
    #define ESPUI_USING_ASYNC 1
    #define ESPUI_USING_FREERTOS 0
    #include <LittleFS.h>
#elif defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RP2040) || defined(RP2040) || defined(TARGET_RP2040) || defined(PICO_RP2040)
    #define ESPUI_PLATFORM "RP2040"
    #define ESPUI_USING_ASYNC 0
    #define ESPUI_USING_FREERTOS 0
    #include <LittleFS.h>
#elif defined(ARDUINO_ARCH_RP2350) || defined(ARDUINO_RP2350) || defined(RP2350) || defined(TARGET_RP2350) || defined(PICO_RP2350)
    #define ESPUI_PLATFORM "RP2350"
    #define ESPUI_USING_ASYNC 0
    #define ESPUI_USING_FREERTOS 0
    #include <LittleFS.h>
#else
    #error "Unsupported platform. ESPUI supports ESP32, ESP8266, RP2040, and RP2350."
#endif

#include <map>

#include "ESPUIcontrol.h"
#include "ESPUIclient.h"

// Platform-specific includes
#if defined(ESP32)
    #include <AsyncTCP.h>
    #include "WiFi.h"
    #include <ESPAsyncWebServer.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/semphr.h>
#elif defined(ESP8266)
    #include <ESPAsyncWebServer.h>
    #include <ArduinoOTA.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
    #include <ESPAsyncTCP.h>
    #include <Hash.h>
#elif defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_RP2350) || defined(ARDUINO_RP2040) || defined(ARDUINO_RP2350) || defined(TARGET_RP2040) || defined(PICO_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2350)
    // RP2040/RP2350: Use RPAsyncTCP for async WebServer (matches ESP-DASH pattern)
    #include <WiFi.h>
    #include <RPAsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    #include <WebServer.h>
    #include <WebSocketsServer.h>
#endif

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
#if ESPUI_USING_FREERTOS
        ControlsSemaphore = xSemaphoreCreateMutex();
        xSemaphoreGive(ControlsSemaphore);
#endif
    }

    // Platform-specific JSON buffer sizes
    // RP2040 has limited RAM (264KB), so use smaller buffers
#if defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RP2040) || defined(ARDUINO_ARCH_RP2350) || defined(ARDUINO_RP2350)
    unsigned int jsonUpdateDocumentSize = 4000;   // Reduced for RP2040/RP2350
    unsigned int jsonInitialDocumentSize = 4000;  // Reduced for RP2040/RP2350
    unsigned int jsonChunkNumberMax = 0;
#elif defined(ESP8266)
    unsigned int jsonUpdateDocumentSize = 2000;
    unsigned int jsonInitialDocumentSize = 2000;
    unsigned int jsonChunkNumberMax = 5;
#else
    unsigned int jsonUpdateDocumentSize = 2000;
    unsigned int jsonInitialDocumentSize = 8000;
    unsigned int jsonChunkNumberMax = 0;
#endif
    bool sliderContinuous = false;

#if ESPUI_USING_ASYNC
    void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);
#else
    // RP2040/RP2350: WebSocket events handled by WebSocketsServer
    void onWsEvent(uint8_t clientId, int type, uint8_t* data, size_t len);
#endif
	bool captivePortal = true;

    void setVerbosity(Verbosity verbosity);
    void begin(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in Memorymode
    void handleClient(); // Poll synchronous server on non-async platforms
    void beginSPIFFS(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in LITTLEFS mode (DEPRECATED, use beginLITTLEFS)
    void beginLITTLEFS(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in LITTLEFS mode

    void prepareFileSystem(bool format = true); // Initially preps the filesystem and loads a lot of
                              // stuff into LITTLEFS
    void list(); // Lists LITTLEFS directory
    void writeFile(const char* path, const char* data);

    uint16_t addControl(ControlType type, const char* label);
    uint16_t addControl(ControlType type, const char* label, const String& value);
    uint16_t addControl(ControlType type, const char* label, const String& value, ControlColor color);
    uint16_t addControl(ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl);
    uint16_t addControl(ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl, std::function<void(Control*, int)> callback);

    bool removeControl(uint16_t id, bool force_rebuild_ui = false);

    // create Elements
    // Create Event Button
    uint16_t button(const char* label, std::function<void(Control*, int)> callback, ControlColor color, const String& value = "");
    uint16_t switcher(const char* label, std::function<void(Control*, int)> callback, ControlColor color, bool startState = false); // Create Toggle Button
    uint16_t pad(const char* label, std::function<void(Control*, int)> callback, ControlColor color); // Create Pad Control
    uint16_t padWithCenter(const char* label, std::function<void(Control*, int)> callback, ControlColor color); // Create Pad Control with Centerbutton
    uint16_t slider(const char* label, std::function<void(Control*, int)> callback, ControlColor color, int value, int min = 0, int max = 100); // Create Slider Control
    uint16_t number(const char* label, std::function<void(Control*, int)> callback, ControlColor color, int value, int min = 0, int max = 100); // Create a Number Input Control
    uint16_t text(const char* label, std::function<void(Control*, int)> callback, ControlColor color, const String& value = ""); // Create a Text Input Control

    // Output only
    uint16_t label(const char* label, ControlColor color,
        const String& value = ""); // Create Label
    uint16_t graph(const char* label, ControlColor color); // Create Graph display
    uint16_t gauge(const char* label, ControlColor color, int value, int min = 0,
        int max = 100); // Create Gauge display
    uint16_t separator(const char* label); //Create separator
    uint16_t fileDisplay(const char* label, ControlColor color, String filename);

    // Input only
    uint16_t accelerometer(const char* label, std::function<void(Control*, int)> callback, ControlColor color);

    // Update Elements

    Control* getControl(uint16_t id);
    Control* getControlNoLock(uint16_t id);

    // Update Elements
    void updateControlValue(uint16_t id, const String& value, int clientId = -1);
    void updateControlValue(Control* control, const String& value, int clientId = -1);

    void updateControlLabel(uint16_t control, const char * value, int clientId = -1);
    void updateControlLabel(Control* control, const char * value, int clientId = -1);

    void updateControl(uint16_t id, int clientId = -1);
    void updateControl(Control* control, int clientId = -1);

    void print(uint16_t id, const String& value);
    void updateLabel(uint16_t id, const String& value);
    void updateButton(uint16_t id, const String& value);
    void updateSwitcher(uint16_t id, bool nValue, int clientId = -1);
    void updateSlider(uint16_t id, int nValue, int clientId = -1);
    void updateNumber(uint16_t id, int nValue, int clientId = -1);
    void updateText(uint16_t id, const String& nValue, int clientId = -1);
    void updateSelect(uint16_t id, const String& nValue, int clientId = -1);
    void updateGauge(uint16_t id, int number, int clientId);
    void updateTime(uint16_t id, int clientId = -1);

    void clearGraph(uint16_t id, int clientId = -1);
    void addGraphPoint(uint16_t id, int nValue, int clientId = -1);

    void setPanelStyle(uint16_t id, const String& style, int clientId = -1);
    void setPanelClass(uint16_t id, const String& pClass, int clientId = -1);
    void setElementStyle(uint16_t id, const String& style, int clientId = -1);
    void setInputType(uint16_t id, const String& type, int clientId = -1);

    void setPanelWide(uint16_t id, bool wide);
    void setVertical(uint16_t id, bool vert = true);
    void setEnabled(uint16_t id, bool enabled = true, int clientId = -1);

    void updateVisibility(uint16_t id, bool visibility, int clientId = -1);

    // Set optional user-defined JavaScript to be included in the UI.
    // js: JavaScript code as a C-string. Must remain valid for the lifetime of the ESPUIClass instance.
    // This is intentionally not a String to avoid dynamic memory allocation.
    void setCustomJS(const char* js);

    // Set optional user-defined CSS to be included in the UI.
    // css: CSS code as a C-string. Must remain valid for the lifetime of the ESPUIClass instance.
    // This is intentionally not a String to avoid dynamic memory allocation.
    void setCustomCSS(const char* css);

    // Variables
    const char* ui_title = "ESPUI"; // Store UI Title and Header Name
    Control* controls = nullptr;
    void jsonReload();
#if defined(ESP32) || defined(ESP8266)
    void jsonDom(uint16_t startidx, AsyncWebSocketClient* client = nullptr, bool Updating = false);
#else
    void jsonDom(uint16_t startidx, void* client = nullptr, bool Updating = false);
#endif

    Verbosity verbosity = Verbosity::Quiet;
    uint32_t  GetNextControlChangeId();
    // emulate former extended callback API by using an intermediate lambda (no deprecation)
    uint16_t addControl(ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl, std::function<void(Control*, int, void*)> callback, void* userData)
    {
        return addControl(type, label, value, color, parentControl, [callback, userData](Control* sender, int type){ callback(sender, type, userData); });
    }
    uint16_t button(const char* label, std::function<void(Control*, int, void*)> callback, ControlColor color, const String& value, void* userData)
    {
        return button(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color, value);
    }
    uint16_t switcher(const char* label, std::function<void(Control*, int, void*)> callback, ControlColor color, bool startState, void* userData)
    {
        return switcher(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color, startState);
    }
    uint16_t pad(const char* label, std::function<void(Control*, int, void*)> callback, ControlColor color, void* userData)
    {
        return pad(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color);
    }
    uint16_t padWithCenter(const char* label, std::function<void(Control*, int, void*)> callback, ControlColor color, void* userData)
    {
        return padWithCenter(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color);
    }
    uint16_t slider(const char* label, std::function<void(Control*, int, void*)> callback, ControlColor color, int value, int min, int max, void* userData)
    {
        return slider(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color, value, min, max);
    }
    uint16_t number(const char* label, std::function<void(Control*, int, void*)> callback, ControlColor color, int value, int min, int max, void* userData)
    {
        return number(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color, value, min, max);
    }
    uint16_t text(const char* label, std::function<void(Control*, int, void*)> callback, ControlColor color, const String& value, void* userData)
    {
        return text(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); } , color, value);
    }
    uint16_t accelerometer(const char* label, std::function<void(Control*, int, void*)> callback, ControlColor color, void* userData)
    {
        return accelerometer(label, [callback, userData](Control* sender, int type){ callback(sender, type, userData); }, color);
    }

    // Async WebServer accessors - only available on ESP32/ESP8266
#if defined(ESP32) || defined(ESP8266)
    AsyncWebServer* WebServer() {return server;}
    AsyncWebSocket* WebSocket() {return ws;}
#else
    void* WebServer() {return nullptr;}
    void* WebSocket() {return nullptr;}
#endif
    size_t clientCount() const {return MapOfClients.size();}

    // LittleFS abstraction for all platforms
#if defined(ESP32)
    #if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
        fs::LittleFSFS & EspuiLittleFS = LittleFS;
    #else
        fs::LITTLEFSFS & EspuiLittleFS = LITTLEFS;
    #endif
#else
    // ESP8266, RP2040, RP2350 all use standard LittleFS
    fs::FS & EspuiLittleFS = LittleFS;
#endif

protected:
    friend class ESPUIclient;
    friend class ESPUIcontrol;

    // Semaphore for thread safety on FreeRTOS platforms
#if ESPUI_USING_FREERTOS
    SemaphoreHandle_t ControlsSemaphore = NULL;
#endif

    void        RemoveToBeDeletedControls();

#if defined(ESP32) || defined(ESP8266)
    AsyncWebServer* server;
    AsyncWebSocket* ws;
#else
    // RP2040/RP2350: No async server/websocket
    void* server;
    WebSocketsServer* ws;
    // Synchronous WebServer for RP2040/RP2350
    ::WebServer* syncServer;
#endif

    const char* basicAuthUsername = nullptr;
    const char* basicAuthPassword = nullptr;
    bool basicAuth = true;
    uint16_t controlCount = 0;

    uint16_t addControl(ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl, Control* control);

#define ClientUpdateType_t ESPUIclient::ClientUpdateType_t
    void NotifyClients(ClientUpdateType_t newState);

    bool SendJsonDocToWebSocket(ArduinoJson::JsonDocument& document, uint16_t clientId);

    std::map<uint32_t, ESPUIclient*> MapOfClients;

    uint32_t    ControlChangeID = 0;
};

extern ESPUIClass ESPUI;
