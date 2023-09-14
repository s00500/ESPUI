#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>

enum ControlType : uint8_t
{
    // fixed Controls
    Title = 0,

    // updatable Controls
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

    Password = 99,
    UpdateOffset = 100,
};

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

class Control
{
public:
    ControlType type;
    uint16_t id; // just mirroring the id here for practical reasons
    const char* label;
    std::function<void(Control*, int)> callback;
    String value;
    ControlColor color;
    bool visible;
    bool wide;
    bool vertical;
    bool enabled;
    uint16_t parentControl;
    String panelStyle;
    String elementStyle;
    String inputType;
    Control* next;

    static constexpr uint16_t noParent = 0xffff;

    Control(ControlType type, 
            const char* label, 
            std::function<void(Control*, int)> callback,
            const String& value, 
            ControlColor color, 
            bool visible, 
            uint16_t parentControl);

    Control(const Control& Control);

    void SendCallback(int type);
    bool HasCallback() { return (nullptr != callback); }
    void MarshalControl(ArduinoJson::JsonObject& item, bool refresh);
    void MarshalErrorMessage(ArduinoJson::JsonObject& item);
    bool ToBeDeleted() { return (ControlSyncState_t::deleted == ControlSyncState); }
    void DeleteControl();
    bool IsUpdated() { return ControlSyncState_t::synchronized != ControlSyncState; }
    void HasBeenUpdated() { ControlSyncState = ControlSyncState_t::updated; }
    void HasBeenSynchronized() {ControlSyncState = ControlSyncState_t::synchronized;}
    void onWsEvent(String& cmd, String& data);

private:
    enum ControlSyncState_t
    {
        synchronized = 0,
        updated,
        deleted,
    };
    ControlSyncState_t ControlSyncState = ControlSyncState_t::synchronized;
};

#define UI_TITLE            ControlType::Title
#define UI_LABEL            ControlType::Label
#define UI_BUTTON           ControlType::Button
#define UI_SWITCHER         ControlType::Switcher
#define UI_PAD              ControlType::Pad
#define UI_CPAD             ControlType::Cpad
#define UI_SLIDER           ControlType::Slider
#define UI_NUMBER           ControlType::Number
#define UI_TEXT_INPUT       ControlType::Text
#define UI_GRAPH            ControlType::Graph
#define UI_ADD_GRAPH_POINT  ControlType::GraphPoint

#define UPDATE_LABEL        ControlType::UpdateLabel
#define UPDATE_SWITCHER     ControlType::UpdateSwitcher
#define UPDATE_SLIDER       ControlType::UpdateSlider
#define UPDATE_NUMBER       ControlType::UpdateNumber
#define UPDATE_TEXT_INPUT   ControlType::UpdateText
#define CLEAR_GRAPH         ControlType::ClearGraph

// Colors
#define COLOR_TURQUOISE     ControlColor::Turquoise
#define COLOR_EMERALD       ControlColor::Emerald
#define COLOR_PETERRIVER    ControlColor::Peterriver
#define COLOR_WETASPHALT    ControlColor::Wetasphalt
#define COLOR_SUNFLOWER     ControlColor::Sunflower
#define COLOR_CARROT        ControlColor::Carrot
#define COLOR_ALIZARIN      ControlColor::Alizarin
#define COLOR_DARK          ControlColor::Dark
#define COLOR_NONE          ControlColor::None
