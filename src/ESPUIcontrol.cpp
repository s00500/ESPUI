#include "ESPUI.h"

static uint16_t idCounter = 0;
static const String ControlError = "*** ESPUI ERROR: Could not transfer control ***";

Control::Control(ControlType type, const char* label, std::function<void(Control*, int)> callback,
    const String& value, ControlColor color, bool visible, uint16_t parentControl)
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
    id = ++idCounter;
    ControlChangeID = 1;
}

Control::Control(const Control& Control)
    : type(Control.type),
        id(Control.id),
        label(Control.label),
        callback(Control.callback),
        value(Control.value),
        color(Control.color),
        visible(Control.visible),
        parentControl(Control.parentControl),
        next(Control.next),
        ControlChangeID(Control.ControlChangeID)
{ }

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

    item[F("label")]   = label;
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
        if (!HasCallback())
        {
            #if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity)
                {
                    Serial.println(String(F("Control::onWsEvent:No callback found for ID ")) + String(id));
                }
            #endif
            break;
        }

        // Serial.println("Control::onWsEvent:Generating callback");
        if (cmd.equals(F("bdown")))
        {
            SendCallback(B_DOWN);
            break;
        }
        
        if (cmd.equals(F("bup")))
        {
            SendCallback(B_UP);
            break;
        }

        if (cmd.equals(F("pfdown")))
        {
            SendCallback(P_FOR_DOWN);
            break;
        }
        
        if (cmd.equals(F("pfup")))
        {
            SendCallback(P_FOR_UP);
            break;
        }

        if (cmd.equals(F("pldown")))
        {
            SendCallback(P_LEFT_DOWN);
            break;
        }

        else if (cmd.equals(F("plup")))
        {
            SendCallback(P_LEFT_UP);
        }
        else if (cmd.equals(F("prdown")))
        {
            SendCallback(P_RIGHT_DOWN);
        }
        else if (cmd.equals(F("prup")))
        {
            SendCallback(P_RIGHT_UP);
        }
        else if (cmd.equals(F("pbdown")))
        {
            SendCallback(P_BACK_DOWN);
        }
        else if (cmd.equals(F("pbup")))
        {
            SendCallback(P_BACK_UP);
        }
        else if (cmd.equals(F("pcdown")))
        {
            SendCallback(P_CENTER_DOWN);
        }
        else if (cmd.equals(F("pcup")))
        {
            SendCallback(P_CENTER_UP);
        }
        else if (cmd.equals(F("sactive")))
        {
            value = "1";
            SendCallback(S_ACTIVE);
        }
        else if (cmd.equals(F("sinactive")))
        {
            value = "0";
            // updateControl(c, client->id());
            SendCallback(S_INACTIVE);
        }
        else if (cmd.equals(F("slvalue")))
        {
            value = data;
            // updateControl(c, client->id());
            SendCallback(SL_VALUE);
        }
        else if (cmd.equals(F("nvalue")))
        {
            value = data;
            // updateControl(c, client->id());
            SendCallback(N_VALUE);
        }
        else if (cmd.equals(F("tvalue")))
        {
            value = data;
            // updateControl(c, client->id());
            SendCallback(T_VALUE);
        }
        else if (cmd.equals(F("tabvalue")))
        {
            SendCallback(0);
        }
        else if (cmd.equals(F("svalue")))
        {
            value = data;
            // updateControl(c, client->id());
            SendCallback(S_VALUE);
        }
        else if (cmd.equals(F("time")))
        {
            value = data;
            // updateControl(c, client->id());
            SendCallback(TM_VALUE);
        }
	    else if (cmd.equals(F("localtime")))
	    {
            value = data;
	        SendCallback(TML_VALUE);
	    }
        else
        {
            #if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity)
                {
                    Serial.println(F("Control::onWsEvent:Malformed message from the websocket"));
                }
            #endif
        }
    } while (false);
}
