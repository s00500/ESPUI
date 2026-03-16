#if !defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_RP2040) && !defined(PICO_RP2040)
#include <DNSServer.h>
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
#endif

#include <ESPUI.h>

// Platform detection for WiFi include
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RP2040) || defined(PICO_RP2040)
#include <WiFi.h>
#include <rp2040.h>
#else
#error "Unsupported platform. ESPUI supports ESP32, ESP8266, and RP2040/RP2350."
#endif

const char* ssid = "ESPUI";
const char* password = "espui";
const char* hostname = "espui";

uint16_t status;
uint16_t button1;
uint16_t millisLabelId;
uint16_t switchOne;

void numberCall(Control* sender, int type)
{
    Serial.println(sender->value);
}

void textCall(Control* sender, int type)
{
    Serial.print("Text: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void slider(Control* sender, int type)
{
    Serial.print("Slider: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void buttonCallback(Control* sender, int type)
{
    switch (type)
    {
    case B_DOWN:
        Serial.println("Button DOWN");
        break;

    case B_UP:
        Serial.println("Button UP");
        break;
    }
}

void buttonExample(Control* sender, int type, void* param)
{
    Serial.print("param: ");
    Serial.println((long)param);
    switch (type)
    {
    case B_DOWN:
        Serial.println("Status: Start");
        ESPUI.updateControlValue(status, "Start");

        ESPUI.getControl(button1)->color = ControlColor::Carrot;
        ESPUI.updateControl(button1);
        break;

    case B_UP:
        Serial.println("Status: Stop");
        ESPUI.updateControlValue(status, "Stop");

        ESPUI.getControl(button1)->color = ControlColor::Peterriver;
        ESPUI.updateControl(button1);
        break;
    }
}

void padExample(Control* sender, int value)
{
    switch (value)
    {
    case P_LEFT_DOWN:
        Serial.print("left down");
        break;

    case P_LEFT_UP:
        Serial.print("left up");
        break;

    case P_RIGHT_DOWN:
        Serial.print("right down");
        break;

    case P_RIGHT_UP:
        Serial.print("right up");
        break;

    case P_FOR_DOWN:
        Serial.print("for down");
        break;

    case P_FOR_UP:
        Serial.print("for up");
        break;

    case P_BACK_DOWN:
        Serial.print("back down");
        break;

    case P_BACK_UP:
        Serial.print("back up");
        break;

    case P_CENTER_DOWN:
        Serial.print("center down");
        break;

    case P_CENTER_UP:
        Serial.print("center up");
        break;
    }

    Serial.print(" ");
    Serial.println(sender->id);
}

void switchExample(Control* sender, int value)
{
    switch (value)
    {
    case S_ACTIVE:
        Serial.print("Active:");
        break;

    case S_INACTIVE:
        Serial.print("Inactive");
        break;
    }

    Serial.print(" ");
    Serial.println(sender->id);
}

void selectExample(Control* sender, int value)
{
    Serial.print("Select: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void otherSwitchExample(Control* sender, int value)
{
    switch (value)
    {
    case S_ACTIVE:
        Serial.print("Active:");
        break;

    case S_INACTIVE:
        Serial.print("Inactive");
        break;
    }

    Serial.print(" ");
    Serial.println(sender->id);
}

void setup(void)
{
    ESPUI.setVerbosity(Verbosity::VerboseJSON);
    Serial.begin(115200);

#if defined(ESP32)
    WiFi.setHostname(hostname);
#elif defined(ESP8266)
    WiFi.hostname(hostname);
#elif defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RP2040) || defined(PICO_RP2040)
    // RP2040 - hostname not supported in standard arduino-pico
#else
    // Other platforms
#endif

    // try to connect to existing network
    WiFi.begin(ssid, password);
    Serial.print("\n\nTry to connect to existing network");

    {
        uint8_t timeout = 10;

        // Wait for connection, 5s timeout
        do
        {
            delay(500);
            Serial.print(".");
            timeout--;
        } while (timeout && WiFi.status() != WL_CONNECTED);

        // not connected -> create hotspot (skip for RP2040)
        if (WiFi.status() != WL_CONNECTED)
        {
#if !defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_RP2040) && !defined(PICO_RP2040)
            Serial.print("\n\nCreating hotspot");

            WiFi.mode(WIFI_AP);
            delay(100);
            WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
#if defined(ESP32)
            uint32_t chipid = 0;
            for (int i = 0; i < 17; i = i + 8)
            {
                chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
            }
#elif defined(ESP8266)
            uint32_t chipid = ESP.getChipId();
#elif defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RP2040) || defined(PICO_RP2040)
            uint32_t chipid = rp2040.getChipID();
#endif
            char ap_ssid[25];
            snprintf(ap_ssid, 26, "ESPUI-%08X", chipid);
            WiFi.softAP(ap_ssid);

            timeout = 5;

            do
            {
                delay(500);
                Serial.print(".");
                timeout--;
            } while (timeout);
#else
            Serial.print("\n\nRP2040 detected - running in simple mode without captive portal");
#endif
        }
    }

#if !defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_RP2040) && !defined(PICO_RP2040)
    dnsServer.start(DNS_PORT, "*", apIP);
#endif

    Serial.println("\n\nWiFi parameters:");
    Serial.print("Mode: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
    Serial.print("IP address: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

#if defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RP2040) || defined(PICO_RP2040)
    // RP2040/RP2350: Use simple WiFi without captive portal
    Serial.println("RP2040/RP2350 detected - running in simple mode");
#endif

    status = ESPUI.addControl(ControlType::Label, "Status:", "Stop", ControlColor::Turquoise);

    uint16_t select1 = ESPUI.addControl(
        ControlType::Select, "Select:", "", ControlColor::Alizarin, Control::noParent, &selectExample);

    ESPUI.addControl(ControlType::Option, "Option1", "Opt1", ControlColor::Alizarin, select1);
    ESPUI.addControl(ControlType::Option, "Option2", "Opt2", ControlColor::Alizarin, select1);
    ESPUI.addControl(ControlType::Option, "Option3", "Opt3", ControlColor::Alizarin, select1);

    ESPUI.addControl(
        ControlType::Text, "Text Test:", "a Text Field", ControlColor::Alizarin, Control::noParent, &textCall);

    millisLabelId = ESPUI.addControl(ControlType::Label, "Millis:", "0", ControlColor::Emerald, Control::noParent);
    button1 = ESPUI.addControl(
        ControlType::Button, "Push Button", "Press", ControlColor::Peterriver, Control::noParent, &buttonCallback);
    ESPUI.addControl(
        ControlType::Button, "Other Button", "Press", ControlColor::Wetasphalt, Control::noParent, &buttonExample, (void*)19);
    ESPUI.addControl(
        ControlType::PadWithCenter, "Pad with center", "", ControlColor::Sunflower, Control::noParent, &padExample);
    ESPUI.addControl(ControlType::Pad, "Pad without center", "", ControlColor::Carrot, Control::noParent, &padExample);
    switchOne = ESPUI.addControl(
        ControlType::Switcher, "Switch one", "", ControlColor::Alizarin, Control::noParent, &switchExample);
    ESPUI.addControl(
        ControlType::Switcher, "Switch two", "", ControlColor::None, Control::noParent, &otherSwitchExample);
    ESPUI.addControl(ControlType::Slider, "Slider one", "30", ControlColor::Alizarin, Control::noParent, &slider);
    ESPUI.addControl(ControlType::Slider, "Slider two", "100", ControlColor::Alizarin, Control::noParent, &slider);
    ESPUI.addControl(ControlType::Number, "Number:", "50", ControlColor::Alizarin, Control::noParent, &numberCall);

    /*
     * .begin loads and serves all files from PROGMEM directly.
     * If you want to serve the files from LITTLEFS use ESPUI.beginLITTLEFS
     * (.prepareFileSystem has to be run in an empty sketch before)
     */

    // Enable this option if you want sliders to be continuous (update during move) and not discrete (update on stop)
    // ESPUI.sliderContinuous = true;

    /*
     * Optionally you can use HTTP BasicAuth. Keep in mind that this is NOT a
     * SECURE way of limiting access.
     * Anyone who is able to sniff traffic will be able to intercept your password
     * since it is transmitted in cleartext. Just add a string as username and
     * password, for example begin("ESPUI Control", "username", "password")
     */

    ESPUI.begin("ESPUI Control");
}

void loop(void)
{
#if !defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_RP2040) && !defined(PICO_RP2040)
    dnsServer.processNextRequest();
#endif

    static long oldTime = 0;
    static bool testSwitchState = false;

    if (millis() - oldTime > 5000)
    {
        ESPUI.updateControlValue(millisLabelId, String(millis()));
        testSwitchState = !testSwitchState;
        ESPUI.updateControlValue(switchOne, testSwitchState ? "1" : "0");

        oldTime = millis();
    }
}
