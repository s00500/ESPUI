#include "ESPUI.h"

static const String ControlError = "*** ESPUI ERROR: Could not transfer control ***";

Control::Control(Control::ControlId_t id, Control::Type type, const char* label, std::function<void(Control*, int)> callback,
    const String& value, Control::Color color, bool visible, ControlId_t parentControl)
    : id(id),
      type(type),
      label(label),
      callback(callback),
      value(value),
      color(color),
      visible(visible),
      wide(false),
      vertical(false),
      enabled(true),
      parentControl(parentControl)
{
    ControlChangeID = 1;
}

Control::Control(const Control& Control)
    : type(Control.type),
      id(Control::noParent),
      label(Control.label),
      callback(Control.callback),
      value(Control.value),
      color(Control.color),
      visible(Control.visible),
      parentControl(Control.parentControl),
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

bool Control::MarshalControl(JsonObject & _item,
                             bool refresh,
                             uint32_t StartingOffset,
                             uint32_t AvailMarshaledLength,
                             uint32_t &EstimatedMarshaledLength)
{
    // this code assumes MaxMarshaledLength > JsonMarshalingRatio
    // Serial.println(String("MarshalControl:           StartingOffset: ") + String(StartingOffset));
    // Serial.println(String("MarshalControl:     AvailMarshaledLength: ") + String(AvailMarshaledLength));
    // Serial.println(String("MarshalControl:               Control ID: ") + String(id));

    bool ControlIsFragmented = false;
    // create a new item in the response document
    JsonObject & item = _item;

    // how much space do we expect to use?
    uint32_t ValueMarshaledLength   = (value.length() - StartingOffset) * JsonMarshalingRatio;
    uint32_t LabelMarshaledLength   = strlen(label) * JsonMarshalingRatio;
    uint32_t MinimumMarshaledLength = LabelMarshaledLength + JsonMarshaledOverhead;
    uint32_t MaximumMarshaledLength = ValueMarshaledLength + MinimumMarshaledLength;
    uint32_t SpaceForMarshaledValue = AvailMarshaledLength - MinimumMarshaledLength;
    // Serial.println(String("MarshalControl:           value.length(): ") + String(value.length()));
    // Serial.println(String("MarshalControl:     ValueMarshaledLength: ") + String(ValueMarshaledLength));
    // Serial.println(String("MarshalControl:     LabelMarshaledLength: ") + String(LabelMarshaledLength));
    // Serial.println(String("MarshalControl:   MaximumMarshaledLength: ") + String(MaximumMarshaledLength));
    // Serial.println(String("MarshalControl:   MinimumMarshaledLength: ") + String(MinimumMarshaledLength));
    // Serial.println(String("MarshalControl:   SpaceForMarshaledValue: ") + String(SpaceForMarshaledValue));

    // will the item fit in the remaining space? Fragment if not
    if (AvailMarshaledLength < MinimumMarshaledLength)
    {
        // Serial.println(String("MarshalControl: Cannot Marshal control. Not enough space for basic headers."));
        EstimatedMarshaledLength = 0;
        return false;
    }

    uint32_t MaxValueLength = (SpaceForMarshaledValue / JsonMarshalingRatio);
    // Serial.println(String("MarshalControl:           MaxValueLength: ") + String(MaxValueLength));

    uint32_t ValueLenToSend = min((value.length() - StartingOffset), MaxValueLength);
    // Serial.println(String("MarshalControl:           ValueLenToSend: ") + String(ValueLenToSend));

    uint32_t AdjustedMarshaledLength = (ValueLenToSend * JsonMarshalingRatio) + MinimumMarshaledLength;
    // Serial.println(String("MarshalControl:  AdjustedMarshaledLength: ") + String(AdjustedMarshaledLength));

    bool NeedToFragment = (ValueLenToSend < value.length());
    // Serial.println(String("MarshalControl:           NeedToFragment: ") + String(NeedToFragment));

    if ((AdjustedMarshaledLength > AvailMarshaledLength) && (0 != ValueLenToSend))
    {
        // Serial.println(String("MarshalControl: Cannot Marshal control. Not enough space for marshaled control."));
        EstimatedMarshaledLength = 0;
        return false;
    }

    EstimatedMarshaledLength = AdjustedMarshaledLength;

    // are we fragmenting?
    if(NeedToFragment || StartingOffset)
    {
        // Serial.println(String("MarshalControl:Start Fragment Processing"));
        // Serial.println(String("MarshalControl:id:                ") + String(id));
        // Serial.println(String("MarshalControl:StartingOffset:    ") + String(StartingOffset));
/*
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
        ControlIsFragmented = true;

        // fill in the fragment header
        _item[F("type")] = uint32_t(Control::Type::Fragment);
        _item[F("id")]   = id;

        // Serial.println(String("MarshalControl:Final length:      ") + String(length));

        _item[F("offset")] = StartingOffset;
        _item[F("length")] = ValueLenToSend;
        _item[F("total")] = value.length();
        AllocateNamedJsonObject(item, _item, F("control"));
    }

    item[F("id")]      = id;
    Control::Type TempType = (Control::Type::Password == type) ? Control::Type::Text : type;
    if(refresh)
    {
        item[F("type")] = uint32_t(TempType) + uint32_t(Control::Type::UpdateOffset);
    }
    else
    {
        item[F("type")] = uint32_t(TempType);
    }

    item[F("label")]   = label;
    item[F ("value")]  = (Control::Type::Password == type) ? F ("--------") : value.substring(StartingOffset, StartingOffset + ValueLenToSend);
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
    if (Control::Type::Option == type)
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

    // Serial.println(String("MarshalControl:Done"));
    return ControlIsFragmented;
}

void Control::MarshalErrorMessage(JsonObject & item)
{
    item[F("id")]      = id;
    item[F("type")]    = uint32_t(Control::Type::Label);
    item[F("label")]   = ControlError.c_str();
    item[F("value")]   = ControlError;
    item[F("visible")] = true;
    item[F("color")]   = (int)Control::Color::Carrot;
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
