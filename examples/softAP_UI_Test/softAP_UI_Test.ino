#include <WiFi.h>
#include <EasyUI.h>

const char* ssid = "LARSUI";
const char* password = "";

long oldTime = 0;

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.setHostname(ssid);
  WiFi.softAP(ssid);
  //WiFi.softAP(ssid, password);
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  EasyUI.label("Status: Maxim Stop");
  EasyUI.label("0");
  EasyUI.button("MaximDance Button", &callback2);
  EasyUI.pad("center", true, &callback3);
  EasyUI.pad("NoCenter", false, &callback3);

  EasyUI.begin("LARS Control");

}

void loop(void) {
  if(millis()-oldTime> 5000){
    EasyUI.print(1, String(millis()));
    oldTime = millis();
    }
}

void callback1(int id, int type) {
  switch (type) {
    case B_DOWN:
      Serial.println("Button DOWN");
      break;
    case B_UP:
      Serial.println("Button UP");
      break;
  }

}

void callback2(int id, int type) {
  switch (type) {
    case B_DOWN:
      Serial.println("Maxim Start DAnce######################");
      EasyUI.print(0, "Status: Maxim Start");
      break;
    case B_UP:
      Serial.println("Maxim STOP DAnce######################");
      EasyUI.print(0, "Status: Maxim Start");
      break;
  }
}
void callback3(int id, int value) {
  switch (value) {
    case P_LEFT_DOWN:
      Serial.println("left down");
      break;
    case P_LEFT_UP:
      Serial.println("left up");
      break;
    case P_RIGHT_DOWN:
      Serial.println("right down");
      break;
    case P_RIGHT_UP:
      Serial.println("right up");
      break;
    case P_FOR_DOWN:
      Serial.println("for down");
      break;
    case P_FOR_UP:
      Serial.println("for up");
      break;
    case P_BACK_DOWN:
      Serial.println("back down");
      break;
    case P_BACK_UP:
      Serial.println("back up");
      break;
    case P_CENTER_DOWN:
      Serial.println("center down");
      break;
    case P_CENTER_UP:
      Serial.println("center up");
      break;
  }
  Serial.println(id);
}
