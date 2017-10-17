#include <WiFi.h>
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

  EasyUI.title("LARSControl");

  EasyUI.label("Label","123");
  EasyUI.label("Label2","456");
  EasyUI.label("Label3","789");
  EasyUI.button("LED", &callback1);
  EasyUI.begin();
}

void loop(void) {
}

void callback1(void) {
  Serial.println("CALLBACK UNO");
}
void callback2(void) {
  Serial.println("CALLBACK DOS");
}
void callback3(void) {
  Serial.println("CALLBACK TRES");
}
