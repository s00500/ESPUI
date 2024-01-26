#include "ESPUI.h"

static uint16_t idCounter = 0;
static const String ControlError = "*** ESPUI ERROR: Could not transfer control ***";

Control::Control(ControlType type, const char* label, std::function<void(Control*, int)> callback, const String& value,
    ControlColor color, bool visible, uint16_t parentControl)
    : label_r(label),
      callback(callback),
      next(nullptr),
      value(value),
      type(type),
      color(color),
      parentControl(parentControl),
      options(CTRL_OPT_ENABLED),
      ControlChangeID(1)
{
    this->visible = visible;
    id = ++idCounter;
}

Control::Control(ControlType type, const __FlashStringHelper* label, std::function<void(Control*, int)> callback,
    const String& value, ControlColor color, bool visible, uint16_t parentControl)
    : label_f(label),
      callback(callback),
      next(nullptr),
      value(value),
      type(type),
      color(color),
      parentControl(parentControl),
      options(CTRL_OPT_ENABLED | CTRL_OPT_LABEL_IN_FLASH),
      ControlChangeID(1)
{
    this->visible = visible;
    id = ++idCounter;
}

Control::Control(const Control& Control)
    : label_r(Control.label_r),
      callback(Control.callback),
      next(Control.next),
      value(Control.value),
      type(Control.type),
      color(Control.color),
      id(Control.id),
      parentControl(Control.parentControl),
      visible(Control.visible),
      ControlChangeID(Control.ControlChangeID)
{
    options = Control.options;
    if (lablel_is_in_flash)
        label_f = Control.label_f;
}

void Control::SendCallback(int type)
{
    if(callback)
    {
        callback(this, type);
    }
}

void Control::DeleteControl() 
{
    _ToBeDeleted = true;
    callback = nullptr;
}

void Control::MarshalControl(JsonObject & _item, bool refresh, uint32_t StartingOffset)
{
    JsonObject & item = _item;
    uint32_t length = value.length();
    uint32_t maxLength = uint32_t(double(ESPUI.jsonInitialDocumentSize) * 0.75);
    if((length > maxLength) || StartingOffset)
    {
    	/*
        Serial.println(String("MarshalControl:Start Fragment Processing"));
        Serial.println(String("MarshalControl:id:                ") + String(id));
        Serial.println(String("MarshalControl:length:            ") + String(length));
        Serial.println(String("MarshalControl:StartingOffset:    ") + String(StartingOffset));
        Serial.println(String("MarshalControl:maxLength:         ") + String(maxLength));

        if(0 == StartingOffset)
        {
            Serial.println(String("MarshalControl: New control to fragement. ID: ") + String(id));
        }
        else
        {
            Serial.println(String("MarshalControl: Next fragement. ID: ") + String(id));
        }
		*/

        // indicate that no additional controls should be sent
        _item[F("type")] = uint32_t(ControlType::Fragment);
        _item[F("id")]   = id;

        length = min((length - StartingOffset), maxLength);
        // Serial.println(String("MarshalControl:Final length:      ") + String(length));

        _item[F("offset")] = StartingOffset;
        _item[F("length")] = length;
        _item[F("total")] = value.length();
        item = _item.createNestedObject(F("control"));
    }

    item[F("id")]      = id;
    ControlType TempType = (ControlType::Password == type) ? ControlType::Text : type;
    if(refresh)
    {
        item[F("type")] = uint32_t(TempType) + uint32_t(ControlType::UpdateOffset);
    }
    else
    {
        item[F("type")] = uint32_t(TempType);
    }

    if (lablel_is_in_flash)
        item[F("label")] = label_f;
    else
        item[F("label")] = label_r;
    item[F ("value")]   = (ControlType::Password == type) ? F ("--------") : value.substring(StartingOffset, length + StartingOffset);
    item[F("visible")] = visible;
    item[F("color")]   = (int)color;
    item[F("enabled")] = enabled;

    if (!panelStyle.isEmpty())    {item[F("panelStyle")]    = panelStyle;}
    if (!elementStyle.isEmpty())  {item[F("elementStyle")]  = elementStyle;}
    if (!inputType.isEmpty())     {item[F("inputType")]     = inputType;}
    if (wide == true)             {item[F("wide")]          = true;}
    if (vertical == true)         {item[F("vertical")]      = true;}
    if (parentControl != Control::noParent)
    {
        item[F("parentControl")] = String(parentControl);
    }

    // special case for selects: to preselect an option, you have to add
    // "selected" to <option>
    if (ControlType::Option == type)
    {
        Control* ParentControl = ESPUI.getControlNoLock(parentControl);
        if (nullptr == ParentControl)
        {
            item[F("selected")] = emptyString;
        }
        else if (ParentControl->value == value)
        {
            item[F("selected")] = F("selected");
        }
        else
        {
            item[F("selected")] = "";
        }
    }
}

void Control::MarshalErrorMessage(JsonObject & item)
{
    item[F("id")]      = id;
    item[F("type")]    = uint32_t(ControlType::Label);
    item[F("label")]   = ControlError.c_str();
    item[F("value")]   = ControlError;
    item[F("visible")] = true;
    item[F("color")]   = (int)ControlColor::Carrot;
    item[F("enabled")] = true;

    if (parentControl != Control::noParent)
    {
        item[F("parentControl")] = String(parentControl);
    }
}

void Control::onWsEvent(String & cmd, String& data)
{
    do // once
    {
        // Serial.println(String(F("Control::onWsEvent")));
        SetControlChangedId(ESPUI.GetNextControlChangeId());

        int arg = 0;

        if (cmd.equals(F("bdown")))
        {
            arg = B_DOWN;
        }
        else if (cmd.equals(F("bup")))
        {
            arg = B_UP;
        }
        else if (cmd.equals(F("pfdown")))
        {
            arg = P_FOR_DOWN;
        }
        else if (cmd.equals(F("pfup")))
        {
            arg = P_FOR_UP;
        }
        else if (cmd.equals(F("pldown")))
        {
            arg = P_LEFT_DOWN;
        }
        else if (cmd.equals(F("plup")))
        {
            arg = P_LEFT_UP;
        }
        else if (cmd.equals(F("prdown")))
        {
            arg = P_RIGHT_DOWN;
        }
        else if (cmd.equals(F("prup")))
        {
            arg = P_RIGHT_UP;
        }
        else if (cmd.equals(F("pbdown")))
        {
            arg = P_BACK_DOWN;
        }
        else if (cmd.equals(F("pbup")))
        {
            arg = P_BACK_UP;
        }
        else if (cmd.equals(F("pcdown")))
        {
            arg = P_CENTER_DOWN;
        }
        else if (cmd.equals(F("pcup")))
        {
            arg = P_CENTER_UP;
        }
        else if (cmd.equals(F("sactive")))
        {
            if (auto_update_value)
                value = "1";
            arg = S_ACTIVE;
        }
        else if (cmd.equals(F("sinactive")))
        {
            if (auto_update_value)
                value = "0";
            arg = S_INACTIVE;
        }
        else if (cmd.equals(F("slvalue")))
        {
            if (auto_update_value)
                value = data;
            arg = SL_VALUE;
        }
        else if (cmd.equals(F("nvalue")))
        {
            if (auto_update_value)
                value = data;
            arg = N_VALUE;
        }
        else if (cmd.equals(F("tvalue")))
        {
            if (auto_update_value)
                value = data;
            arg = T_VALUE;
        }
        else if (cmd.equals(F("tabvalue")))
        {
            arg = 0;
        }
        else if (cmd.equals(F("svalue")))
        {
            if (auto_update_value)
                value = data;
            arg = S_VALUE;
        }
        else if (cmd.equals(F("time")))
        {
            if (auto_update_value)
                value = data;
            arg = TM_VALUE;
        }
        else
        {
        #if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.println(F("Control::onWsEvent:Malformed message from the websocket"));
            }
        #endif
            break;
        }

        if (!HasCallback())
        {
#if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity)
            {
                Serial.println(String(F("Control::onWsEvent:No callback found for ID ")) + String(id));
            }
#endif
        }
        else
        {
            // Serial.println("Control::onWsEvent:Generating callback");
            SendCallback(arg);
        }

    } while (false);
}
