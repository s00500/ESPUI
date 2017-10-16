#ifndef EasyUI_h
#define EasyUI_h

#define HARDWARE "esp32"

//ifdef 8266
//#include "Hash.h"

#include "Arduino.h"
#include "stdlib_noniso.h"
#include "ArduinoJson.h"
#include "FS.h"
#include "SPIFFS.h"
#include "WiFi.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

typedef struct Button
{
    const char *label;
    void (*callback)();
} Button;

class EasyUIClass{

public:
  void begin();   // Begin HTTP Server + WebSocketsServer & Initalize All Elements
  void title(const char* _title);  // Define Webpage Header Name and title
  //void toggleButton(uint8_t  pin, const char* tbutton_label, int start_state = 0, bool swap_state = false);   // Create Toggle Button
  void button(const char* tbutton_label, void(* callBack)());   // Create Event Button
  void label(const char* label_name, const char*  label_val); // Create Label
  // Variables ---
  const char* ui_title = "EasyUI"; // Store UI Title and Header Name
  int bIndex;   // How Many Buttons
  int l_index;    // How Many Labels

  bool tbutton_swap[10];
  Button* buttons[10];

  const char*  label_value[10];      // Stores Label Values - MAX 10
  const char* label_title[10];    // Stores Label Titles - MAX 10

  String webpage;                      // Coverts Arduino elements to JSON elements

  void tbClick(String _index, String _status);
  void tbuttonStatus();
  void jsonDom(AsyncWebSocketClient * client);

 private:
  AsyncWebServer* server;
  AsyncWebSocket* ws;
};

extern EasyUIClass EasyUI;
#endif
