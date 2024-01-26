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

    Fragment,
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

#define CTRL_OPT_ENABLED                    0x01
#define CTRL_OPT_VISIBLE                    0x02
#define CTRL_OPT_LABEL_IN_FLASH             0x04
#define CTRL_OPT_AUTO_UPDATE_VALUE          0x08
#define CTRL_OPT_WIDE                       0x10
#define CTRL_OPT_VERTICAL                   0x20

class Control
{
public:
    static constexpr uint16_t noParent = 0xffff;
    
    // Pointers
    union
    {
        const char* label_r;
        const __FlashStringHelper* label_f;
    };    
    std::function<void(Control*, int)> callback;
    Control* next;

    // Strings
    String value;
    String panelStyle;
    String elementStyle;
    String inputType;
    
    //enums
    ControlType type;
    ControlColor color;

    // uint16_t
    uint16_t id; // just mirroring the id here for practical reasons
    uint16_t parentControl;

    union
    {
        struct
        {
            uint16_t enabled : 1 ; 
            uint16_t visible : 1; 
            uint16_t lablel_is_in_flash : 1;
            uint16_t auto_update_value : 1;
            uint16_t wide : 1;
            uint16_t vertical : 1;
        };
        uint16_t options;
    };

    Control(ControlType type, 
            const char* label, 
            std::function<void(Control*, int)> callback,
            const String& value, 
            ControlColor color, 
            bool visible, 
            uint16_t parentControl);
    
    Control(ControlType type, 
            const __FlashStringHelper* label, 
            std::function<void(Control*, int)> callback, 
            const String& value,
            ControlColor color, 
            bool visible, 
            uint16_t parentControl);

    Control(const Control& Control);

    void SendCallback(int type);
    bool HasCallback() { return (nullptr != callback); }
    void MarshalControl(ArduinoJson::JsonObject& item, bool refresh, uint32_t DataOffset);
    void MarshalErrorMessage(ArduinoJson::JsonObject& item);
    void DeleteControl();
    void onWsEvent(String& cmd, String& data);
    inline bool ToBeDeleted() { return _ToBeDeleted; }
    inline bool NeedsSync(uint32_t lastControlChangeID) {return (false == _ToBeDeleted) && (lastControlChangeID < ControlChangeID);}
    void    SetControlChangedId(uint32_t value) {ControlChangeID = value;}
    
private:
    bool _ToBeDeleted = false;
    uint32_t ControlChangeID;
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
