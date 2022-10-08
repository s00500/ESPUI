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

uint16_t Control::idCounter = 1;

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

#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		File root = LittleFS.open(dirname);
	#else
		File root = LITTLEFS.open(dirname);
	#endif
#else
    File root = LittleFS.open(dirname);
#endif

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

    Dir dir = LittleFS.openDir(dirname);

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
#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
	if (!LittleFS.begin())
	#else
    if (!LITTLEFS.begin())
	#endif
    {
        Serial.println(F("LITTLEFS Mount Failed"));
        return;
    }
#else
    if (!LittleFS.begin())
    {
        Serial.println(F("LittleFS Mount Failed"));
        return;
    }
#endif

    listDir("/", 1);
#if defined(ESP32)

	Serial.print(F("Total KB: "));
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		Serial.println(LittleFS.totalBytes()/1024);
	#else
		Serial.println(LITTLEFS.totalBytes()/1024);
	#endif
	Serial.print(F("Used KB: "));
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		Serial.println(LittleFS.usedBytes()/1024);
	#else
		Serial.println(LITTLEFS.usedBytes()/1024);
	#endif

#else
    FSInfo fs_info;
    LittleFS.info(fs_info);

	Serial.print(F("Total KB: "));
    Serial.println(fs_info.totalBytes/1024);
	Serial.print(F("Used KB: "));
    Serial.println(fs_info.usedBytes/1024);

#endif
}

void deleteFile(const char* path)
{
#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		bool exists = LittleFS.exists(path);
	#else
		bool exists = LITTLEFS.exists(path);
	#endif
#else
    bool exists = LittleFS.exists(path);
#endif

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

#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		bool didRemove = LittleFS.remove(path);
	#else
		bool didRemove = LITTLEFS.remove(path);
	#endif
#else
    bool didRemove = LittleFS.remove(path);
#endif
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

void writeFile(const char* path, const char* data)
{
#if defined(DEBUG_ESPUI)
    if (ESPUI.verbosity)
    {
        Serial.printf_P(PSTR("Writing file: %s\n"), path);
    }
#endif

#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		File file = LittleFS.open(path, FILE_WRITE);
	#else
		File file = LITTLEFS.open(path, FILE_WRITE);
	#endif
#else
    File file = LittleFS.open(path, FILE_WRITE);
#endif

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
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("File written"));
        }
#endif
    }
    else
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("Write failed"));
        }
#endif
    }

#else

    if (file.print(FPSTR(data)))
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("File written"));
        }
#endif
    }
    else
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("Write failed"));
        }
#endif
    }

#endif
    file.close();
}

// end LITTLEFS functions

void ESPUIClass::prepareFileSystem(bool format)
{
    // this function should only be used once

#if defined(DEBUG_ESPUI)
    if (this->verbosity)
    {
        Serial.println(F("About to prepare filesystem..."));
    }
#endif
#if defined(ESP32)
	if(format)
	{
		#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
			//LittleFS.format();	//It would seem this causes a crash so commented out for now. An unformatted partition will be automatically formatted fine.
		#else
			//LITTLEFS.format();	//It would seem this causes a crash so commented out for now. An unformatted partition will be automatically formatted fine.
		#endif
	}
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
	if(!LittleFS.begin(true))
	#else
	if(!LITTLEFS.begin(true))
	#endif
	{
#if defined(DEBUG_ESPUI)
		if (this->verbosity)
		{
			Serial.println(F("LITTLEFS Mount Failed"));
		}
#endif

		return;
	}

#if defined(DEBUG_ESPUI)
	if (this->verbosity)
	{
		listDir("/", 1);
		Serial.println(F("LITTLEFS Mount ESP32 Done"));
	}
#endif
#else
	if(format)
	{
		LittleFS.format();
	}
    LittleFS.begin();

#if defined(DEBUG_ESPUI)
    if (this->verbosity)
    {
        Serial.println(F("LITTLEFS Mount ESP8266 Done"));
    }
#endif

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
    if (this->verbosity)
    {
        Serial.println(F("Cleanup done"));
    }
#endif

    // Now write
	#ifdef ESP32
		#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		writeFile("/index.htm", HTML_INDEX);
		LittleFS.mkdir("/css");
		writeFile("/css/style.css", CSS_STYLE);
		writeFile("/css/normalize.css", CSS_NORMALIZE);
		LittleFS.mkdir("/js");
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
    if (this->verbosity)
    {
        Serial.println(F("Done Initializing filesystem :-)"));
    }
#endif

#if defined(ESP32)

#if defined(DEBUG_ESPUI)
    if (this->verbosity)
    {
        listDir("/", 1);
    }
#endif

#endif

#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
	LittleFS.end();
	#else
    LITTLEFS.end();
	#endif
#else
    LittleFS.end();
#endif
}

// Handle Websockets Communication
void onWsEvent(
    AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len)
{
    switch (type)
    {
    case WS_EVT_DISCONNECT: {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.print(F("Disconnected!\n"));
        }
#endif

        break;
    }

    case WS_EVT_PONG: {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.print(F("Received PONG!\n"));
        }
#endif

        break;
    }

    case WS_EVT_ERROR: {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.print(F("WebSocket Error!\n"));
        }
#endif

        break;
    }

    case WS_EVT_CONNECT: {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.print(F("Connected: "));
            Serial.println(client->id());
        }
#endif

        ESPUI.jsonDom(0, client);

#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("JSON Data Sent to Client!"));
        }
#endif
    }
    break;

    case WS_EVT_DATA: {
        String msg = "";
        msg.reserve(len + 1);

        for (size_t i = 0; i < len; i++)
        {
            msg += (char)data[i];
        }

        if (msg.startsWith(F("uiok:")))
        {
            int idx = msg.substring(msg.indexOf(':') + 1).toInt();
            ESPUI.jsonDom(idx, client);
        } else 
        {
            uint16_t id = msg.substring(msg.lastIndexOf(':') + 1).toInt();

    #if defined(DEBUG_ESPUI)
            if (ESPUI.verbosity >= Verbosity::VerboseJSON)
            {
                Serial.print(F("WS rec: "));
                Serial.println(msg);
                Serial.print(F("WS recognised ID: "));
                Serial.println(id);
            }
    #endif

            Control* c = ESPUI.getControl(id);

            if (c == nullptr)
            {
    #if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity)
                {
                    Serial.print(F("No control found for ID "));
                    Serial.println(id);
                }
    #endif

                return;
            }

            if (false == c->HasCallback())
            {
#if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity)
                {
                    Serial.print(F("No callback found for ID "));
                    Serial.println(id);
                }
    #endif

                return;
            }

            if (msg.startsWith(F("bdown:")))
            {
                c->SendCallback(B_DOWN);
            }
            else if (msg.startsWith(F("bup:")))
            {
                c->SendCallback(B_UP);
            }
            else if (msg.startsWith(F("pfdown:")))
            {
                c->SendCallback(P_FOR_DOWN);
            }
            else if (msg.startsWith(F("pfup:")))
            {
                c->SendCallback(P_FOR_UP);
            }
            else if (msg.startsWith(F("pldown:")))
            {
                c->SendCallback(P_LEFT_DOWN);
            }
            else if (msg.startsWith(F("plup:")))
            {
                c->SendCallback(P_LEFT_UP);
            }
            else if (msg.startsWith(F("prdown:")))
            {
                c->SendCallback(P_RIGHT_DOWN);
            }
            else if (msg.startsWith(F("prup:")))
            {
                c->SendCallback(P_RIGHT_UP);
            }
            else if (msg.startsWith(F("pbdown:")))
            {
                c->SendCallback(P_BACK_DOWN);
            }
            else if (msg.startsWith(F("pbup:")))
            {
                c->SendCallback(P_BACK_UP);
            }
            else if (msg.startsWith(F("pcdown:")))
            {
                c->SendCallback(P_CENTER_DOWN);
            }
            else if (msg.startsWith(F("pcup:")))
            {
                c->SendCallback(P_CENTER_UP);
            }
            else if (msg.startsWith(F("sactive:")))
            {
                c->value = "1";
                ESPUI.updateControl(c, client->id());
                c->SendCallback(S_ACTIVE);
            }
            else if (msg.startsWith(F("sinactive:")))
            {
                c->value = "0";
                ESPUI.updateControl(c, client->id());
                c->SendCallback(S_INACTIVE);
            }
            else if (msg.startsWith(F("slvalue:")))
            {
                c->value = msg.substring(msg.indexOf(':') + 1, msg.lastIndexOf(':'));
                ESPUI.updateControl(c, client->id());
                c->SendCallback(SL_VALUE);
            }
            else if (msg.startsWith(F("nvalue:")))
            {
                c->value = msg.substring(msg.indexOf(':') + 1, msg.lastIndexOf(':'));
                ESPUI.updateControl(c, client->id());
                c->SendCallback(N_VALUE);
            }
            else if (msg.startsWith(F("tvalue:")))
            {
                c->value = msg.substring(msg.indexOf(':') + 1, msg.lastIndexOf(':'));
                ESPUI.updateControl(c, client->id());
                c->SendCallback(T_VALUE);
            }
            else if (msg.startsWith("tabvalue:"))
            {
                c->SendCallback(client->id());
            }
            else if (msg.startsWith(F("svalue:")))
            {
                c->value = msg.substring(msg.indexOf(':') + 1, msg.lastIndexOf(':'));
                ESPUI.updateControl(c, client->id());
                c->SendCallback(S_VALUE);
            }
            else if (msg.startsWith(F("time:")))
            {
                c->value = msg.substring(msg.indexOf(':') + 1, msg.lastIndexOf(':'));
                ESPUI.updateControl(c, client->id());
                c->SendCallback(TM_VALUE);
            }
            else
            {
    #if defined(DEBUG_ESPUI)
                if (ESPUI.verbosity)
                {
                    Serial.println(F("Malformated message from the websocket"));
                }
    #endif
            }
        }
    }
    break;

    default:
        break;
    }
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label)
{
    return addControl(type, label, String(""));
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label, const String& value)
{
    return addControl(type, label, value, ControlColor::Turquoise);
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label, const String& value, ControlColor color)
{
    return addControl(type, label, value, color, Control::noParent);
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl)
{
    return addControl(type, label, value, color, parentControl, nullptr);
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl, void (*callback)(Control*, int))
{
    uint16_t id = addControl(type, label, value, color, parentControl, nullptr, nullptr);
    // set the original style callback
    getControl(id)->callback = callback;
    return id;
}

uint16_t ESPUIClass::addControl(ControlType type, const char* label, const String& value, ControlColor color, uint16_t parentControl, void (*callback)(Control*, int, void *), void * UserData)
{
    Control* control = new Control(type, label, callback, UserData, value, color, true, parentControl);

    if (this->controls == nullptr)
    {
        this->controls = control;
    }
    else
    {
        Control* iterator = this->controls;

        while (iterator->next != nullptr)
        {
            iterator = iterator->next;
        }

        iterator->next = control;
    }

    this->controlCount++;

    return control->id;
}

bool ESPUIClass::removeControl(uint16_t id, bool force_reload_ui)
{
    Control* PreviousControl = nullptr;
    Control* CurrentControl  = this->controls;

    while(nullptr != CurrentControl)
    {
        if (id == CurrentControl->id)
        {
            break;
        }
        PreviousControl = CurrentControl;
        CurrentControl = CurrentControl->next;
    }

    if (nullptr != CurrentControl)
    {
        if(nullptr == PreviousControl)
        {
            this->controls = CurrentControl->next;
        }
        else
        {
            PreviousControl->next = CurrentControl->next;
        }
        delete CurrentControl;
        this->controlCount--;
        if (force_reload_ui)
        {
            jsonReload();
        }
        else
        {
            jsonDom(0); // resends to all
        }
        return true;
    }

    return false;
}

uint16_t ESPUIClass::label(const char* label, ControlColor color, const String& value)
{
    return addControl(ControlType::Label, label, value, color);
}

uint16_t ESPUIClass::graph(const char* label, ControlColor color)
{
    return addControl(ControlType::Graph, label, "", color);
}

uint16_t ESPUIClass::slider(const char* label, void (*callback)(Control*, int), ControlColor color, int value, int min, int max)
{
    uint16_t id = slider(label, nullptr, color, value, min, max, nullptr);
    getControl(id)->callback = callback;
    return id;
}

uint16_t ESPUIClass::slider(const char* label, void (*callback)(Control*, int, void*), ControlColor color, int value, int min, int max, void* userData)
{
    uint16_t sliderId = addControl(ControlType::Slider, label, String(value), color, Control::noParent, callback, userData);
    addControl(ControlType::Min, label, String(min), ControlColor::None, sliderId);
    addControl(ControlType::Max, label, String(max), ControlColor::None, sliderId);

    return sliderId;
}

uint16_t ESPUIClass::button(const char* label, void (*callback)(Control*, int), ControlColor color, const String& value)
{
    return addControl(ControlType::Button, label, value, color, Control::noParent, callback);
}

uint16_t ESPUIClass::button(const char* label, void (*callback)(Control*, int, void*), ControlColor color, const String& value, void* UserData)
{
    return addControl(ControlType::Button, label, value, color, Control::noParent, callback, UserData);
}

uint16_t ESPUIClass::switcher(const char* label, void (*callback)(Control*, int), ControlColor color, bool startState)
{
    return addControl(ControlType::Switcher, label, startState ? "1" : "0", color, Control::noParent, callback);
}

uint16_t ESPUIClass::switcher(const char* label, void (*callback)(Control*, int, void*), ControlColor color, bool startState, void* UserData)
{
    return addControl(ControlType::Switcher, label, startState ? "1" : "0", color, Control::noParent, callback, UserData);
}

uint16_t ESPUIClass::pad(const char* label, void (*callback)(Control*, int), ControlColor color)
{
    return addControl(ControlType::Pad, label, "", color, Control::noParent, callback);
}

uint16_t ESPUIClass::pad(const char* label, void (*callback)(Control*, int, void*), ControlColor color, void* UserData)
{
    return addControl(ControlType::Pad, label, "", color, Control::noParent, callback, UserData);
}

uint16_t ESPUIClass::padWithCenter(const char* label, void (*callback)(Control*, int), ControlColor color)
{
    return addControl(ControlType::PadWithCenter, label, "", color, Control::noParent, callback);
}

uint16_t ESPUIClass::padWithCenter(const char* label, void (*callback)(Control*, int, void*), ControlColor color, void* UserData)
{
    return addControl(ControlType::PadWithCenter, label, "", color, Control::noParent, callback, UserData);
}

uint16_t ESPUIClass::number(const char* label, void (*callback)(Control*, int), ControlColor color, int number, int min, int max)
{
    uint16_t numberId = addControl(ControlType::Number, label, String(number), color, Control::noParent, callback);
    addControl(ControlType::Min, label, String(min), ControlColor::None, numberId);
    addControl(ControlType::Max, label, String(max), ControlColor::None, numberId);
    return numberId;
}

uint16_t ESPUIClass::number(const char* label, void (*callback)(Control*, int, void*), ControlColor color, int number, int min, int max, void* UserData)
{
    uint16_t numberId = addControl(ControlType::Number, label, String(number), color, Control::noParent, callback, UserData);
    addControl(ControlType::Min, label, String(min), ControlColor::None, numberId);
    addControl(ControlType::Max, label, String(max), ControlColor::None, numberId);
    return numberId;
}

uint16_t ESPUIClass::gauge(const char* label, ControlColor color, int number, int min, int max)
{
    uint16_t numberId = addControl(ControlType::Gauge, label, String(number), color, Control::noParent);
    addControl(ControlType::Min, label, String(min), ControlColor::None, numberId);
    addControl(ControlType::Max, label, String(max), ControlColor::None, numberId);
    return numberId;
}

uint16_t ESPUIClass::separator(const char* label) {
    return addControl(ControlType::Separator, label, "", ControlColor::Alizarin, Control::noParent, nullptr);
}

uint16_t ESPUIClass::accelerometer(const char* label, void (*callback)(Control*, int), ControlColor color)
{
    return addControl(ControlType::Accel, label, "", color, Control::noParent, callback);
}

uint16_t ESPUIClass::accelerometer(const char* label, void (*callback)(Control*, int, void*), ControlColor color, void* UserData)
{
    return addControl(ControlType::Accel, label, "", color, Control::noParent, callback, UserData);
}

uint16_t ESPUIClass::text(const char* label, void (*callback)(Control*, int), ControlColor color, const String& value)
{
    return addControl(ControlType::Text, label, value, color, Control::noParent, callback);
}

uint16_t ESPUIClass::text(const char* label, void (*callback)(Control*, int, void*), ControlColor color, const String& value, void* UserData)
{
    return addControl(ControlType::Text, label, value, color, Control::noParent, callback, UserData);
}

Control* ESPUIClass::getControl(uint16_t id)
{
    Control* control = this->controls;

    while (control != nullptr)
    {
        if (control->id == id)
        {
            return control;
        }

        control = control->next;
    }

    return nullptr;
}

void ESPUIClass::updateControl(Control* control, int clientId)
{
    if (!control)
    {
        return;
    }

    if (this->ws == nullptr)
    {
        return;
    }

    String json;
    DynamicJsonDocument document(jsonUpdateDocumentSize);
    JsonObject root = document.to<JsonObject>();

    root["type"] = (int)control->type + ControlType::UpdateOffset;
    root["value"] = control->value;
    root["id"] = control->id;
    root["visible"] = control->visible;
    root["color"] = (int)control->color;
    root["enabled"] = control->enabled;
    if (control->panelStyle.length())
        root["panelStyle"] = control->panelStyle;
    if (control->elementStyle.length())
        root["elementStyle"] = control->elementStyle;
    if (control->inputType.length())
        root["inputType"] = control->inputType;
    serializeJson(document, json);

#if defined(DEBUG_ESPUI)
    if (this->verbosity >= Verbosity::VerboseJSON)
    {
        Serial.println(json);
    }
#endif

    if (clientId < 0)
    {
#if defined(DEBUG_ESPUI)
        if (this->verbosity >= Verbosity::VerboseJSON)
        {
            Serial.println(F("TextAll"));
        }
#endif
        this->ws->textAll(json);
        return;
    }
    // This is a hacky workaround because ESPAsyncWebServer does not have a
    // function like this and it's clients array is private
    int tryId = 0;

    for (size_t count = 0; count < this->ws->count();)
    {
        if (this->ws->hasClient(tryId))
        {
            if (clientId != tryId)
            {
                this->ws->client(tryId)->text(json);
            }

            count++;
        }

        tryId++;
    }
}

void ESPUIClass::setPanelStyle(uint16_t id, String style, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->panelStyle = style;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setElementStyle(uint16_t id, String style, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->elementStyle = style;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setInputType(uint16_t id, String type, int clientId)
{
    Control* control = getControl(id);
    if (control)
    {
        control->inputType = type;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setPanelWide(uint16_t id, bool wide)
{
    Control* control = getControl(id);
    if (control)
    {
        control->wide = wide;
    }
}

void ESPUIClass::setEnabled(uint16_t id, bool enabled, int clientId) {
    Control* control = getControl(id);
    if (control)
    {
        control->enabled = enabled;
        updateControl(control, clientId);
    }
}

void ESPUIClass::setVertical(uint16_t id, bool vert) {
    Control* control = getControl(id);
    if (control)
    {
        control->vertical = vert;
    }
}

void ESPUIClass::updateControl(uint16_t id, int clientId)
{
    Control* control = getControl(id);

    if (!control)
    {
#if defined(DEBUG_ESPUI)
        if (this->verbosity)
        {
            Serial.printf_P(PSTR("Error: There is no control with ID %d"), id);
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

void ESPUIClass::updateControlValue(uint16_t id, const String& value, int clientId)
{
    Control* control = getControl(id);

    if (!control)
    {
#if defined(DEBUG_ESPUI)
        if (this->verbosity)
        {
            Serial.printf_P(PSTR("Error: There is no control with ID %d"), id);
        }
#endif
        return;
    }

    updateControlValue(control, value, clientId);
}

void ESPUIClass::updateVisibility(uint16_t id, bool visibility, int clientId) {
    Control* control = getControl(id);
    if(control)
    {
        control->visible = visibility;
        updateControl(id);
    }
}

void ESPUIClass::print(uint16_t id, const String& value)
{
    updateControlValue(id, value);
}

void ESPUIClass::updateLabel(uint16_t id, const String& value)
{
    updateControlValue(id, value);
}

void ESPUIClass::updateButton(uint16_t id, const String& value) {
    updateControlValue(id, value);
}

void ESPUIClass::updateSlider(uint16_t id, int nValue, int clientId)
{
    updateControlValue(id, String(nValue), clientId);
}

void ESPUIClass::updateSwitcher(uint16_t id, bool nValue, int clientId)
{
    updateControlValue(id, String(nValue ? "1" : "0"), clientId);
}

void ESPUIClass::updateNumber(uint16_t id, int number, int clientId)
{
    updateControlValue(id, String(number), clientId);
}

void ESPUIClass::updateText(uint16_t id, const String& text, int clientId)
{
    updateControlValue(id, text, clientId);
}

void ESPUIClass::updateSelect(uint16_t id, const String& text, int clientId)
{
    updateControlValue(id, text, clientId);
}

void ESPUIClass::updateGauge(uint16_t id, int number, int clientId)
{
    updateControlValue(id, String(number), clientId);
}

void ESPUIClass::updateTime(uint16_t id, int clientId) 
{
    updateControl(id, clientId);
}

void ESPUIClass::clearGraph(uint16_t id, int clientId) { }

void ESPUIClass::addGraphPoint(uint16_t id, int nValue, int clientId)
{
    Control* control = getControl(id);
    if (!control)
    {
        return;
    }

    String json;
    DynamicJsonDocument document(jsonUpdateDocumentSize);
    JsonObject root = document.to<JsonObject>();

    root["type"] = (int)ControlType::GraphPoint;
    root["value"] = nValue;
    root["id"] = control->id;
    serializeJson(document, json);

#if defined(DEBUG_ESPUI)
    if (this->verbosity >= Verbosity::VerboseJSON)
    {
        Serial.println(json);
    }
#endif

    if (clientId < 0)
    {
        this->ws->textAll(json);
        return;
    }
    // This is a hacky workaround because ESPAsyncWebServer does not have a
    // function like this and it's clients array is private
    int tryId = 0;

    for (size_t count = 0; count < this->ws->count();)
    {
        if (this->ws->hasClient(tryId))
        {
            if (clientId != tryId)
            {
                this->ws->client(tryId)->text(json);

#if defined(DEBUG_ESPUI)
                if (this->verbosity >= Verbosity::VerboseJSON)
                {
                    Serial.println(json);
                }
#endif
            }

            count++;
        }

        tryId++;
    }
}

/*
Convert & Transfer Arduino elements to JSON elements. This function sends a chunk of 
JSON describing the controls of the UI, starting from the control at index startidx.
If startidx is 0 then a UI_INITIAL_GUI message will be sent, else a UI_EXTEND_GUI.
Both message types contain a list of serialised UI elements. Only a portion of the UI
will be sent in order to avoid websocket buffer overflows. The client will acknowledge 
receipt of a partial message by requesting the next chunk of UI.

The protocol is:
SERVER: jsonDom(0): 
    "UI_INITIAL_GUI: n serialised UI elements"
CLIENT: controls.js:handleEvent()
    "uiok:n"
SERVER: jsonDom(n):
    "UI_EXTEND_GUI: n serialised UI elements"
CLIENT: controls.js:handleEvent()
    "uiok:2*n"
etc. 
*/
void ESPUIClass::jsonDom(uint16_t startidx, AsyncWebSocketClient* client)
{
    if(startidx >= this->controlCount)
    {
        return;
    }

    DynamicJsonDocument document(jsonInitialDocumentSize);
    document["type"] = startidx ? (int)UI_EXTEND_GUI : (int)UI_INITIAL_GUI;
    document["sliderContinuous"] = sliderContinuous;
    document["startindex"] = startidx;
    document["totalcontrols"] = this->controlCount;
    JsonArray items = document.createNestedArray("controls");
    JsonObject titleItem = items.createNestedObject();
    titleItem["type"] = (int)UI_TITLE;
    titleItem["label"] = ui_title;

    prepareJSONChunk(client, startidx, &items);

    String json;
    serializeJson(document, json);
#if defined(DEBUG_ESPUI)
    if (this->verbosity >= Verbosity::VerboseJSON)
    {
        Serial.println("Sending elements --------->");
        Serial.println(json);
    }
#endif
    if (client != nullptr)
        client->text(json);
    else
        this->ws->textAll(json);
}

/* Prepare a chunk of elements as a single JSON string. If the allowed number of elements is greater than the total
number this will represent the entire UI. More likely, it will represent a small section of the UI to be sent. The client
will acknoledge receipt by requesting the next chunk. */
void ESPUIClass::prepareJSONChunk(AsyncWebSocketClient* client, uint16_t startindex, JsonArray* items)
{
    //First check that there will be sufficient nodes in the list
    if(startindex >= this->controlCount) 
    {
        return;
    }

    //Follow the list until control points to the startindex'th node
    Control* control = this->controls;
    for(auto i = 0; i < startindex; i++) {
        control = control->next;
    }

    //To prevent overflow, keep track of the number of elements we have serialised into this message
    int elementcount = 0;
    while (control != nullptr && elementcount < 10)
    {
        JsonObject item = items->createNestedObject();

        item["id"] = String(control->id);
        item["type"] = (int)control->type;
        item["label"] = control->label;
        item["value"] = String(control->value);
        item["color"] = (int)control->color;
        item["visible"] = (int)control->visible;
        item["enabled"] = control->enabled;
        if (control->panelStyle.length())
            item["panelStyle"] = String(control->panelStyle);
        if (control->elementStyle.length())
            item["elementStyle"] = String(control->elementStyle);
        if (control->inputType.length())
            item["inputType"] = String(control->inputType);
        if (control->wide == true)
            item["wide"] = true;
        if (control->vertical == true)
            item["vertical"] = true;

        if (control->parentControl != Control::noParent)
        {
            item["parentControl"] = String(control->parentControl);
        }

        // special case for selects: to preselect an option, you have to add
        // "selected" to <option>
        if (control->type == ControlType::Option)
        {
            if (ESPUI.getControl(control->parentControl)->value == control->value)
            {
                item["selected"] = "selected";
            }
            else
            {
                item["selected"] = "";
            }
        }

        control = control->next;
        elementcount++;
    }
    return;
}

void ESPUIClass::jsonReload()
{
    String json;
    DynamicJsonDocument document(jsonUpdateDocumentSize);
    JsonObject root = document.to<JsonObject>();

    root["type"] = (int)UI_RELOAD;
    serializeJson(document, json);

#if defined(DEBUG_ESPUI)
    if (this->verbosity >= Verbosity::VerboseJSON)
    {
        Serial.println(json);
    }
#endif

    this->ws->textAll(json);
}

void ESPUIClass::beginSPIFFS(const char* _title, const char* username, const char* password, uint16_t port)
{
    // Backwards compatibility wrapper
    beginLITTLEFS(_title, username, password, port);
}

void ESPUIClass::beginLITTLEFS(const char* _title, const char* username, const char* password, uint16_t port)
{
    ui_title = _title;
    this->basicAuthUsername = username;
    this->basicAuthPassword = password;

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

#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
	bool fsBegin = LittleFS.begin();
	#else
    bool fsBegin = LITTLEFS.begin();
	#endif
#else
    bool fsBegin = LittleFS.begin();
#endif
    if (!fsBegin)
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("LITTLEFS Mount Failed, PLEASE CHECK THE README ON HOW TO "
                             "PREPARE YOUR ESP!!!!!!!"));
        }
#endif

        return;
    }

#if defined(DEBUG_ESPUI)
    if (ESPUI.verbosity)
    {
        listDir("/", 1);
    }
#endif

#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
	bool indexExists = LittleFS.exists("/index.htm");
	#else
    bool indexExists = LITTLEFS.exists("/index.htm");
	#endif
#else
    bool indexExists = LittleFS.exists("/index.htm");
#endif
    if (!indexExists)
    {
#if defined(DEBUG_ESPUI)
        if (ESPUI.verbosity)
        {
            Serial.println(F("Please read the README!!!!!!!, Make sure to "
                             "ESPUI.prepareFileSystem() once in an empty sketch"));
        }
#endif

        return;
    }

    ws->onEvent(onWsEvent);
    server->addHandler(ws);

    if (basicAuth)
    {
        if (WS_AUTHENTICATION)
        {
            ws->setAuthentication(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword);
        }
#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		server->serveStatic("/", LittleFS, "/").setDefaultFile("index.htm").setAuthentication(username, password);
	#else
        server->serveStatic("/", LITTLEFS, "/").setDefaultFile("index.htm").setAuthentication(username, password);
	#endif
#else
        server->serveStatic("/", LittleFS, "/").setDefaultFile("index.htm").setAuthentication(username, password);
#endif
    }
    else
    {
#if defined(ESP32)
	#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
		server->serveStatic("/", LittleFS, "/").setDefaultFile("index.htm");
	#else
        server->serveStatic("/", LITTLEFS, "/").setDefaultFile("index.htm");
	#endif
#else
        server->serveStatic("/", LittleFS, "/").setDefaultFile("index.htm");
#endif
    }

    // Heap for general Servertest
    server->on("/heap", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (ESPUI.basicAuth && !request->authenticate(ESPUI.basicAuthUsername, ESPUI.basicAuthPassword))
        {
            return request->requestAuthentication();
        }

        request->send(200, "text/plain", String(ESP.getFreeHeap()) + " In LITTLEFS mode");
    });

    server->onNotFound([this](AsyncWebServerRequest* request) {
		if(captivePortal)
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
    if (this->verbosity)
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

    ws->onEvent(onWsEvent);
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

        request->send(200, "text/plain", String(ESP.getFreeHeap()) + " In Memorymode");
    });

    server->onNotFound([this](AsyncWebServerRequest* request) {
		if(captivePortal)
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
    if (this->verbosity)
    {
        Serial.println(F("UI Initialized"));
    }
#endif
}

void ESPUIClass::setVerbosity(Verbosity v)
{
    this->verbosity = v;
}

void Control::SendCallback(int type)
{
    if(callback)
    {
        callback(this, type);
    }

    if (extendedCallback)
    {
        extendedCallback(this, type, user);
    }
}

ESPUIClass ESPUI;
