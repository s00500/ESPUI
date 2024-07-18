#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>

class Control
{
public:
    enum Type : uint8_t
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
        FileDisplay,

        Fragment = 98,
        Password = 99,
        UpdateOffset = 100,
    };

    enum Color : uint8_t
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

    typedef uint16_t ControlId_t;

    const char* label;
    std::function<void(Control*, int)> callback;
    String value;
    Color color;
    bool visible;
    bool wide;
    bool vertical;
    bool enabled;
    ControlId_t parentControl;
    String panelStyle;
    String elementStyle;
    String inputType;

    static constexpr uint16_t noParent = 0xffff;

    Control(ControlId_t id,
            Type type,
            const char* label,
            std::function<void(Control*, int)> callback,
            const String& value,
            Color color,
            bool visible,
            uint16_t parentControl);

    Control(const Control& Control);

    void SendCallback(int type);
    bool HasCallback() { return (nullptr != callback); }
    bool MarshalControl(ArduinoJson::JsonObject& item, bool refresh, uint32_t DataOffset, uint32_t MaxLength, uint32_t & EstimmatedUsedSpace);
    void MarshalErrorMessage(ArduinoJson::JsonObject& item);
    void DeleteControl();
    void onWsEvent(String& cmd, String& data);
    inline bool ToBeDeleted() { return _ToBeDeleted; }
    inline bool NeedsSync(uint32_t lastControlChangeID) {return (false == _ToBeDeleted) && (lastControlChangeID < ControlChangeID);}
    void    SetControlChangedId(uint32_t value) {ControlChangeID = value;}
    inline ControlId_t GetId() {return id;}
    inline Type        GetType() {return type;}

#define UI_TITLE            Control::Type::Title
#define UI_LABEL            Control::Type::Label
#define UI_BUTTON           Control::Type::Button
#define UI_SWITCHER         Control::Type::Switcher
#define UI_PAD              Control::Type::Pad
#define UI_CPAD             Control::Type::Cpad
#define UI_SLIDER           Control::Type::Slider
#define UI_NUMBER           Control::Type::Number
#define UI_TEXT_INPUT       Control::Type::Text
#define UI_GRAPH            Control::Type::Graph
#define UI_ADD_GRAPH_POINT  Control::Type::GraphPoint

#define UPDATE_LABEL        Control::Type::UpdateLabel
#define UPDATE_SWITCHER     Control::Type::UpdateSwitcher
#define UPDATE_SLIDER       Control::Type::UpdateSlider
#define UPDATE_NUMBER       Control::Type::UpdateNumber
#define UPDATE_TEXT_INPUT   Control::Type::UpdateText
#define CLEAR_GRAPH         Control::Type::ClearGraph

// Colors
#define COLOR_TURQUOISE     Control::Color::Turquoise
#define COLOR_EMERALD       Control::Color::Emerald
#define COLOR_PETERRIVER    Control::Color::Peterriver
#define COLOR_WETASPHALT    Control::Color::Wetasphalt
#define COLOR_SUNFLOWER     Control::Color::Sunflower
#define COLOR_CARROT        Control::Color::Carrot
#define COLOR_ALIZARIN      Control::Color::Alizarin
#define COLOR_DARK          Control::Color::Dark
#define COLOR_NONE          Control::Color::None

private:
    Type type = Type::Title;
    ControlId_t id = Control::noParent;

    bool _ToBeDeleted = false;
    uint32_t ControlChangeID = 0;
    String OldValue = emptyString;

    // multiplier for converting a typical controller label or value to a Json object
    #define JsonMarshalingRatio 3
    // Marshaed Control overhead length
    #define JsonMarshaledOverhead 64
};

