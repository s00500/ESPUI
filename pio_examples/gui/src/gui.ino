#include <DNSServer.h>
#include <ESPUI.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

#if defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

const char* ssid = "YourNetworkName";
const char* password = "YourNetworkPassphrase";

const char* hostname = "espui";

#ifdef TEST_FILEDISPLAY
String DisplayTestFileName = "/FileName.txt";
int fileDisplayId = Control::noParent;
#endif // def TEST_FILEDISPLAY

int statusLabelId = Control::noParent;

#ifdef TEST_GRAPH
int graphId = Control::noParent;
#endif // def TEST_GRAPH
int millisLabelId = Control::noParent;
int testSwitchId = Control::noParent;

#ifdef TEST_HUGE_TEXT
char HugeText[1025];
#endif // def TEST_HUGE_TEXT

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
    // Like all Control Values in ESPUI slider values are Strings. To use them as int simply do this:
    int sliderValueWithOffset = sender->value.toInt() + 100;
    Serial.print("SliderValue with offset");
    Serial.println(sliderValueWithOffset);
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
    Serial.println(String("param: ") + String(long(param)));
    switch (type)
    {
    case B_DOWN:
        Serial.println("Status: Start");
        ESPUI.print(statusLabelId, "Start");
        break;

    case B_UP:
        Serial.println("Status: Stop");
        ESPUI.print(statusLabelId, "Stop");
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

#ifdef TEST_HUGE_TEXT
    memset(HugeText, 0x0, sizeof(HugeText));
    memset(HugeText, 'a', sizeof(HugeText)-1);
#endif // def TEST_HUGE_TEXT
    
#if defined(ESP32)
    WiFi.setHostname(hostname);
#else
    WiFi.hostname(hostname);
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

        // not connected -> create hotspot
        if (WiFi.status() != WL_CONNECTED)
        {
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
#else
            uint32_t chipid = ESP.getChipId();
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
        }
    }

    dnsServer.start(DNS_PORT, "*", apIP);

    Serial.println("\n\nWiFi parameters:");
    Serial.print("Mode: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
    Serial.print("IP address: ");
    Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

    statusLabelId = ESPUI.label("Status:", ControlColor::Turquoise, "Stop");
    millisLabelId = ESPUI.label("Millis:", ControlColor::Emerald, "0");
    ESPUI.button("Push Button", &buttonCallback, ControlColor::Peterriver, "Press");
    ESPUI.button("Other Button", &buttonExample, ControlColor::Wetasphalt, "Press", (void*)19);
    ESPUI.padWithCenter("Pad with center", &padExample, ControlColor::Sunflower);
    ESPUI.pad("Pad without center", &padExample, ControlColor::Carrot);
    testSwitchId = ESPUI.switcher("Switch one", &switchExample, ControlColor::Alizarin, false);
    ESPUI.switcher("Switch two", &otherSwitchExample, ControlColor::None, true);
    ESPUI.slider("Slider one", &slider, ControlColor::Alizarin, 30, 0, 30);
    ESPUI.slider("Slider two", &slider, ControlColor::None, 100);
    ESPUI.text("Text Test:", &textCall, ControlColor::Alizarin, "a Text Field");

#ifdef TEST_HUGE_TEXT
    ESPUI.text("Huge Text Test:", &textCall, ControlColor::Alizarin, HugeText);
#endif // def TEST_HUGE_TEXT

    ESPUI.number("Numbertest", &numberCall, ControlColor::Alizarin, 5, 0, 10);

#ifdef TEST_FILEDISPLAY
    fileDisplayId = ESPUI.fileDisplay("Filetest", ControlColor::Turquoise, DisplayTestFileName);
#endif // def TEST_FILEDISPLAY

#ifdef TEST_GRAPH
    graphId = ESPUI.graph("Graph Test", ControlColor::Wetasphalt);
#endif // def TEST_GRAPH

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
    ESPUI.sliderContinuous = true;

#ifdef TEST_FILEDISPLAY
    ESPUI.prepareFileSystem();

    // create a text file
    ESPUI.writeFile("/DisplayFile.txt", "Test Line\n");

    // these files are used by browsers to auto config a connection.
    ESPUI.writeFile("/wpad.dat", " ");
    ESPUI.writeFile("/connecttest.txt", " ");
#endif // def TEST_FILEDISPLAY

    ESPUI.beginLITTLEFS("ESPUI Control");
}

void loop(void)
{
    dnsServer.processNextRequest();

    static long oldTime = 0;
    static bool testSwitchState = false;
    delay(10);

    if (millis() - oldTime > 5000)
    {
        ESPUI.print(millisLabelId, String(millis()));

#ifdef TEST_GRAPH
        ESPUI.addGraphPoint(graphId, random(1, 50));
#endif // def TEST_GRAPH

        testSwitchState = !testSwitchState;
        ESPUI.updateSwitcher(testSwitchId, testSwitchState);

#ifdef TEST_FILEDISPLAY
        // update the file Display file.
        File testFile = ESPUI.EspuiLittleFS.open(String("/") + DisplayTestFileName, "a");
        uint32_t filesize = testFile.size();

        String TestLine = String("Current Time = ") + String(millis()) + "\n";
        if(filesize < 1000)
        {
            testFile.write((const uint8_t*)TestLine.c_str(), TestLine.length());
            ESPUI.updateControl(fileDisplayId);
            
            TestLine += String("filesize: ") + String(filesize);
            // Serial.println(TestLine);
        }
        testFile.close();
#endif // def TEST_FILEDISPLAY

        oldTime = millis();
    }
}
