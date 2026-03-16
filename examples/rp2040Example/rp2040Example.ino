/*
 * ESPUI Example for Raspberry Pi Pico W (RP2040)
 * 
 * NOTE: RP2040 uses synchronous WebServer instead of async WebSocket.
 * This means:
 * - Real-time UI updates are via polling (slower than WebSocket)
 * - Some latency in control responses
 * 
 * Board: Raspberry Pi Pico W with arduino-pico core
 * 
 * Instructions:
 * 1. Install arduino-pico core in PlatformIO:
 *    https://earlephilhower.github.io/arduino-pico/core_platformio.txt
 * 2. Set upload port to your Pico's COM port
 * 3. Build and upload
 */

#include <Arduino.h>
#include <ESPUI.h>

// WiFi credentials - UPDATE THESE
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

void setup()
{
    // Serial for debugging
    Serial.begin(115200);
    delay(1000);

    // Initialize WiFi
    Serial.print("Connecting to WiFi...");
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
    Serial.println("Using Async WebServer (ESP32/ESP8266)");
#else
    Serial.println("Using Synchronous WebServer (RP2040/RP2350)");
    Serial.println("NOTE: UI updates use polling, not WebSocket");
#endif

    // Initialize ESPUI
    // This example uses memory mode (no filesystem needed)
    ESPUI.begin("RP2040 Control");
    
    // Create some basic controls
    // Note: Controls work but update latency is higher than ESP32/ESP8266
    
    // A simple label
    ESPUI.label("Status:", ControlColor::None, "RP2040 Ready");
    
    // A button
    ESPUI.button("Test Button", [](Control* sender, int value)
    {
        Serial.printf("Button pressed! Value: %d\n", value);
        ESPUI.updateLabel(sender->id, "Button Pressed!");
    }, ControlColor::Peterriver);

    // A slider
    ESPUI.slider("Test Slider", [](Control* sender, int value)
    {
        Serial.printf("Slider value: %d\n", value);
    }, ControlColor::Emerald, 50, 0, 100);

    // A text input
    ESPUI.text("Test Text", [](Control* sender, int value)
    {
        Serial.printf("Text input: %s\n", sender->value.c_str());
    }, ControlColor::Sunflower, "Hello RP2040!");

    Serial.println("ESPUI initialized!");
    Serial.print("Open http://");
    Serial.print(WiFi.localIP());
    Serial.println(" in your browser");
}

void loop()
{
    // Required for RP2040/RP2350 sync server; no-op on async platforms.
    ESPUI.handleClient();

    // Other loop code here
    delay(10);
}
