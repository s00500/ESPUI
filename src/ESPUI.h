#ifndef ESPUI_h
#define ESPUI_h

#define DEBUG_ESPUI true
#define WS_AUTHENTICATION false

#include "Arduino.h"
#include "ArduinoJson.h"
#include "stdlib_noniso.h"

#if defined(ESP32)
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LITTLEFS.h>

#include "WiFi.h"

#else

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Hash.h>
#include <LittleFS.h>

#define FILE_WRITE "w"

#endif

// Message Types (and control types)

enum ControlType : uint8_t
{
    // fixed controls
    Title = 0,

    // updatable controls
    Pad,
    PadWithCenter,
    Button,
    Label,
    Switcher,
    Slider,
    Number,
    Text,
    Graph,
    GraphPoint,
    Tab,
    Select,
    Option,
    Min,
    Max,
    Step,
    Gauge,
    Accel,
    Separator,
    Time,

    UpdateOffset = 100,
    UpdatePad = 101,
    UpdatePadWithCenter,
    ButtonButton,
    UpdateLabel,
    UpdateSwitcher,
    UpdateSlider,
    UpdateNumber,
    UpdateText,
    ClearGraph,
    UpdateTab,
    UpdateSelection,
    UpdateOption,
    UpdateMin,
    UpdateMax,
    UpdateStep,
    UpdateGauge,
    UpdateAccel,
    UpdateSeparator,
    UpdateTime,

    InitialGui = 200,
    Reload = 201,
    ExtendGUI = 210
};

#define UI_INITIAL_GUI ControlType::InitialGui
#define UI_RELOAD ControlType::Reload
#define UI_EXTEND_GUI ControlType::ExtendGUI

#define UI_TITLE ControlType::Title
#define UI_LABEL ControlType::Label
#define UI_BUTTON ControlType::Button
#define UI_SWITCHER ControlType::Switcher
#define UI_PAD ControlType::Pad
#define UI_CPAD ControlType::Cpad
#define UI_SLIDER ControlType::Slider
#define UI_NUMBER ControlType::Number
#define UI_TEXT_INPUT ControlType::Text
#define UI_GRAPH ControlType::Graph
#define UI_ADD_GRAPH_POINT ControlType::GraphPoint

#define UPDATE_LABEL ControlType::UpdateLabel
#define UPDATE_SWITCHER ControlType::UpdateSwitcher
#define UPDATE_SLIDER ControlType::UpdateSlider
#define UPDATE_NUMBER ControlType::UpdateNumber
#define UPDATE_TEXT_INPUT ControlType::UpdateText
#define CLEAR_GRAPH ControlType::ClearGraph

// Colors
enum ControlColor : uint8_t
{
    Turquoise,
    Emerald,
    Peterriver,
    Wetasphalt,
    Sunflower,
    Carrot,
    Alizarin,
    Dark,
    None = 0xFF
};
#define COLOR_TURQUOISE ControlColor::Turquoise
#define COLOR_EMERALD ControlColor::Emerald
#define COLOR_PETERRIVER ControlColor::Peterriver
#define COLOR_WETASPHALT ControlColor::Wetasphalt
#define COLOR_SUNFLOWER ControlColor::Sunflower
#define COLOR_CARROT ControlColor::Carrot
#define COLOR_ALIZARIN ControlColor::Alizarin
#define COLOR_DARK ControlColor::Dark
#define COLOR_NONE ControlColor::None

class Control
{
public:
    ControlType type;
    uint16_t id; // just mirroring the id here for practical reasons
    const char* label;
    void (*callback)(Control*, int);
    String value;
    ControlColor color;
    bool visible;
    bool wide;
    bool vertical;
    bool enabled;
    uint16_t parentControl;
    String panelStyle;
    String elementStyle;
    Control* next;

    static constexpr uint16_t noParent = 0xffff;

    Control(ControlType type, const char* label, void (*callback)(Control*, int), const String& value,
        ControlColor color, bool visible = true, uint16_t parentControl = Control::noParent)
        : type(type),
          label(label),
          callback(callback),
          value(value),
          color(color),
          visible(visible),
          wide(false),
          vertical(false),
          enabled(true),
          parentControl(parentControl),
          next(nullptr)
    {
        id = idCounter++;
    }

    Control(const Control& control)
        : type(control.type),
          id(control.id),
          label(control.label),
          callback(control.callback),
          value(control.value),
          color(control.color),
          visible(control.visible),
          parentControl(control.parentControl),
          next(control.next)
    { }

private:
    static uint16_t idCounter;
};

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
        verbosity = Verbosity::Quiet;
        jsonUpdateDocumentSize = 2000;
        jsonInitialDocumentSize = 8000;
        sliderContinuous = false;
    }
    unsigned int jsonUpdateDocumentSize;
    unsigned int jsonInitialDocumentSize;
    bool sliderContinuous;

    void setVerbosity(Verbosity verbosity);
    void begin(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in Memorymode
    void beginSPIFFS(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in LITTLEFS mode (DEPRECATED, use beginLITTLEFS)
    void beginLITTLEFS(const char* _title, const char* username = nullptr, const char* password = nullptr,
        uint16_t port = 80); // Setup server and page in LITTLEFS mode

    void prepareFileSystem(); // Initially preps the filesystem and loads a lot of
                              // stuff into LITTLEFS
    void list(); // Lists LITTLEFS directory

    uint16_t addControl(ControlType type, const char* label, const String& value = String(""),
        ControlColor color = ControlColor::Turquoise, uint16_t parentControl = Control::noParent,
        void (*callback)(Control*, int) = nullptr);
    bool removeControl(uint16_t id, bool force_reload_ui = false);

    // create Elements
    uint16_t button(const char* label, void (*callback)(Control*, int), ControlColor color,
        const String& value = ""); // Create Event Button
    uint16_t switcher(const char* label, void (*callback)(Control*, int), ControlColor color,
        bool startState = false); // Create Toggle Button
    uint16_t pad(const char* label, void (*callback)(Control*, int),
        ControlColor color); // Create Pad Control
    uint16_t padWithCenter(const char* label, void (*callback)(Control*, int),
        ControlColor color); // Create Pad Control with Centerbutton

    uint16_t slider(const char* label, void (*callback)(Control*, int), ControlColor color, int value, int min = 0,
        int max = 100); // Create Slider Control
    uint16_t number(const char* label, void (*callback)(Control*, int), ControlColor color, int value, int min = 0,
        int max = 100); // Create a Number Input Control
    uint16_t text(const char* label, void (*callback)(Control*, int), ControlColor color,
        const String& value = ""); // Create a Text Input Control

    // Output only
    uint16_t label(const char* label, ControlColor color,
        const String& value = ""); // Create Label
    uint16_t graph(const char* label, ControlColor color); // Create Graph display
    uint16_t gauge(const char* label, ControlColor color, int value, int min = 0,
        int max = 100); // Create Gauge display
    uint16_t separator(const char* label); //Create separator

    // Input only
    uint16_t accelerometer(const char* label, void (*callback)(Control*, int), ControlColor color);

    // Update Elements

    Control* getControl(uint16_t id);

    // Update Elements
    void updateControlValue(uint16_t id, const String& value, int clientId = -1);
    void updateControlValue(Control* control, const String& value, int clientId = -1);

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

    void setPanelStyle(uint16_t id, String style, int clientId = -1);
    void setElementStyle(uint16_t id, String style, int clientId = -1);

    void setPanelWide(uint16_t id, bool wide);
    void setVertical(uint16_t id, bool vert = true);
    void setEnabled(uint16_t id, bool enabled = true, int clientId = -1);

    void updateVisibility(uint16_t id, bool visibility, int clientId = -1);

    // Variables
    const char* ui_title = "ESPUI"; // Store UI Title and Header Name
    Control* controls = nullptr;
    void jsonReload();
    void jsonDom(uint16_t startidx, AsyncWebSocketClient* client = nullptr);

    Verbosity verbosity;

    AsyncWebServer* server;
    AsyncWebSocket* ws;

private:
    const char* basicAuthUsername = nullptr;
    const char* basicAuthPassword = nullptr;
    bool basicAuth = true;

    uint16_t controlCount = 0;

    void prepareJSONChunk(AsyncWebSocketClient* client, uint16_t startindex, JsonArray* items);
};

extern ESPUIClass ESPUI;
#endif
