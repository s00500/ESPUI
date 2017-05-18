/*
 * Author:  Ayush Sharma (ayushsharma82) Github
 * Library: EasyUI
 * - This is a Test Code of EasyUI Library
 * - It Will Use the OnBoard LED of ESP8266 and toggle it.
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EasyUI.h>

const char* ssid = "hotspotk4c";
const char* password = "frenzy99";

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  EasyUI.label("Label","123");
  EasyUI.toggleButton(D4, "LED", HIGH, true);   // LED Toggle Button * D4 for Nodemcu , 2 for Generic
  EasyUI.begin();
}

void loop(void) {
  EasyUI.loop();
}
