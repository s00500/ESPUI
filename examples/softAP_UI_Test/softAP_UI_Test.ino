/*
 * Author:  Ayush Sharma (ayushsharma82) Github
 * Library: EasyUI
 * - This is a Test Code of EasyUI Library
 * - It Will Use the OnBoard LED of ESP8266 and toggle it.
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EasyUI.h>

const char* ssid = "EasyUI";
const char* password = "";

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  EasyUI.label("Label","123");
  EasyUI.toggleButton(2, "LED", HIGH, true);   // LED Toggle Button
  EasyUI.begin();
}

void loop(void) {
  EasyUI.loop();
}
