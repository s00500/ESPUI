/*
 * ESPUI Minimal Example for Raspberry Pi Pico W (RP2040)
 * 
 * A simple GUI test for RP2040/RP2350 support.
 * This demonstrates basic ESPUI controls working on Pico W.
 * 
 * Hardware: Raspberry Pi Pico W
 * Framework: Arduino with arduino-pico core
 * 
 * Setup Instructions:
 * 1. Install arduino-pico core in Arduino IDE:
 *    https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
 * 2. Open this project in Arduino IDE 
 * 3. Edit WiFi credentials below
 * 4. Build and upload to Pico W
 * 5. Open the IP address shown in Serial Monitor
 */

#include <Arduino.h>
#include <ESPUI.h>

// WiFi credentials - UPDATE THESE FOR YOUR NETWORK
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Control IDs
uint16_t statusLabelId = 0;
uint16_t button1Id = 0;
uint16_t button2Id = 0;
uint16_t slider1Id = 0;
uint16_t switch1Id = 0;

void setup()
{
    // Serial for debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== ESPUI Pico GUI Test ===\n");

    // Initialize WiFi
    Serial.print("Connecting to WiFi...");
    Serial.println(ssid);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.print("WiFi connected! IP: ");
    Serial.println(WiFi.localIP());

#if ESPUI_USING_ASYNC
    Serial.println("Mode: Async WebServer (ESP32/ESP8266)");
#else
    Serial.println("Mode: Synchronous WebServer (RP2040/RP2350)");
#endif

    // ===== Create Controls =====
    
    // Status Label - shows current state
    statusLabelId = ESPUI.label("Status:", ControlColor::None, "Ready");
    
    // Button 1 - toggles LED/test
    button1Id = ESPUI.button("Toggle LED", [](Control* sender, int value)
    {
        Serial.printf("Button 1 pressed! Value: %d\n", value);
        
        if (value == B_DOWN)
        {
            ESPUI.updateLabel(statusLabelId, "Button Pressed!");
            Serial.println("LED ON simulation");
        }
        else
        {
            ESPUI.updateLabel(statusLabelId, "Ready");
            Serial.println("LED OFF simulation");
        }
    }, ControlColor::Peterriver);

    // Button 2 - another test button
    button2Id = ESPUI.button("Test Action", [](Control* sender, int value)
    {
        Serial.printf("Button 2 pressed! Value: %d\n", value);
        ESPUI.updateLabel(statusLabelId, "Test Action!");
    }, ControlColor::Emerald);

    // Slider - test slider control
    slider1Id = ESPUI.slider("Brightness", [](Control* sender, int value)
    {
        Serial.printf("Slider value: %d\n", value);
        char buf[32];
        snprintf(buf, sizeof(buf), "Brightness: %d%%", value);
        ESPUI.updateLabel(statusLabelId, buf);
    }, ControlColor::Sunflower, 50, 0, 100);

    // Switch/Checkbox - toggle
    switch1Id = ESPUI.switcher("Enable Feature", [](Control* sender, int value)
    {
        Serial.printf("Switch value: %d\n", value);
        if (value == S_ACTIVE)
        {
            ESPUI.updateLabel(statusLabelId, "Feature Enabled");
        }
        else
        {
            ESPUI.updateLabel(statusLabelId, "Feature Disabled");
        }
    }, ControlColor::Turquoise, false);

    // Start ESPUI after controls are created so the initial UI build includes all components.
    // On RP2040, ESPUI serves UI files from LittleFS and auto-prepares missing files.
    ESPUI.begin("Pico GUI Test");

    Serial.println("\n=== ESPUI Controls Created ===");
    Serial.print("Open http://");
    Serial.print(WiFi.localIP());
    Serial.println(" in your browser\n");
}

void loop()
{
    // Required for RP2040/RP2350 sync server; no-op on async platforms.
    ESPUI.handleClient();

    // Small delay to prevent watchdog issues
    delay(10);
}
