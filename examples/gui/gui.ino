#include <ESPUI.h>

#if defined(ESP32)
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

const char *ssid = "ESPUI";
const char *password = "";

long oldTime = 0;
bool switchi = false;

void slider(Control sender, int type) {
  Serial.println(sender.value);
}

void buttonCallback(Control sender, int type) {
  switch (type) {
  case B_DOWN:
    Serial.println("Button DOWN");
    break;
  case B_UP:
    Serial.println("Button UP");
    break;
  }
}

void buttonExample(Control sender, int type) {
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
void padExample(Control sender, int value) {
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
  Serial.println(sender.id);
}

void switchExample(Control sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active:");
    break;
  case S_INACTIVE:
    Serial.print("Inactive");
    break;
  }
  Serial.print(" ");
  Serial.println(sender.id);
}

void otherSwitchExample(Control sender, int value) {
  switch (value) {
  case S_ACTIVE:
    Serial.print("Active:");
    break;
  case S_INACTIVE:
    Serial.print("Inactive");
    break;
  }
  Serial.print(" ");
  Serial.println(sender.id);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);

  #if defined(ESP32)
    WiFi.setHostname(ssid);
  #else
    WiFi.hostname(ssid);
  #endif

  WiFi.softAP(ssid);
  // WiFi.softAP(ssid, password);
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

  ESPUI.label("Status:", COLOR_TURQUOISE, "Stop");
  ESPUI.label("Millis:", COLOR_EMERALD, "0");
  ESPUI.button("Push Button", &buttonCallback, COLOR_PETERRIVER);
  ESPUI.button("Other Button", &buttonExample, COLOR_WETASPHALT, "Press");
  ESPUI.pad("Pad with center", true, &padExample, COLOR_SUNFLOWER);
  ESPUI.pad("Pad without center", false, &padExample, COLOR_CARROT);
  ESPUI.switcher("Switch one", false, &switchExample, COLOR_ALIZARIN);
  ESPUI.switcher("Switch two", true, &otherSwitchExample, COLOR_NONE);
  ESPUI.slider("Slider one", &slider, COLOR_ALIZARIN, "30");
  ESPUI.slider("Slider two", &slider, COLOR_NONE, "100");

  ESPUI.begin("ESP32 Control");
}

void loop(void) {
  if (millis() - oldTime > 5000) {
    ESPUI.print("Millis:", String(millis()));
    switchi = !switchi;
    ESPUI.updateSwitcher("Switch one", switchi);
    oldTime = millis();
  }
}
