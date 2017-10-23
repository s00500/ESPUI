#include <WiFi.h>
#include <ESPUI.h>

const char* ssid = "ESP32";
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

  // change the beginning to this if you want to join an existing network
  /*
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
  */

  ESPUI.label("Status: Stop");
  ESPUI.label("0");
  ESPUI.button("Push Button", &buttonCallback);
  ESPUI.button("Push Button", &buttonExample);
  ESPUI.pad("center", true, &padExample);
  ESPUI.pad("NoCenter", false, &padExample);
  ESPUI.switcher("Switch one", false, &switchExample);
  ESPUI.switcher("Switch two", true, &otherSwitchExample);

  ESPUI.begin("ESP32 Control");

}

void loop(void) {
  if (millis() - oldTime > 5000) {
    ESPUI.print(1, String(millis()));
    oldTime = millis();
  }
}

void buttonCallback(int id, int type) {
  switch (type) {
    case B_DOWN:
      Serial.println("Button DOWN");
      break;
    case B_UP:
      Serial.println("Button UP");
      break;
  }

}

void buttonExample(int id, int type) {
  switch (type) {
    case B_DOWN:
      Serial.println("Status: Start");
      ESPUI.print(0, "Status: Start");
      break;
    case B_UP:
      Serial.println("Status: Stop");
      ESPUI.print(0, "Status: Stop");
      break;
  }
}
void padExample(int id, int value) {
  switch (value) {
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
  Serial.println(id);
}

void switchExample(int id, int value) {
  switch (value) {
    case S_ACTIVE:
      Serial.print("Active:");
      break;
    case S_INACTIVE:
      Serial.print("Inactive");
      break;
  }
  Serial.print(" ");
  Serial.println(id);
}

void otherSwitchExample(int id, int value) {
  switch (value) {
    case S_ACTIVE:
      Serial.print("Active:");
      break;
    case S_INACTIVE:
      Serial.print("Inactive");
      break;
  }
  Serial.print(" ");
  Serial.println(id);
}
