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


class EasyUIClass{

public:
  void begin();   // Begin HTTP Server + WebSocketsServer & Initalize All Elements
  void title(const char* _title);  // Define Webpage Header Name and title
  void toggleButton(uint8_t  pin, const char* tbutton_label, int start_state = 0, bool swap_state = false);   // Create Toggle Button
  void button(uint8_t  pin, const char* tbutton_label, int start_state = 0, bool swap_state = false);   // Create Toggle Button
  void label(const char* label_name, const char*  label_val); // Create Label
  // Variables ---
  const char* ui_title = "EasyUI"; // Store UI Title and Header Name
  int tb_index;   // Calculate How Many Toggle Buttons
  int l_index;    // Calculate How Many Labels

  bool tbutton_swap[10];
  uint8_t  tbutton_pinout[10];   // Stores GPIO Values - MAX 10
  const char*  label_value[10];      // Stores Label Values - MAX 10

  const char* tbuttontitle[10];   // Stores Toggle Button Titles - MAX 10
  const char* label_title[10];    // Stores Label Titles - MAX 10

  // const char* variable_type[10];   // un-used feature for now  // Stores Label Types, Like 'C' , 'F' or '%' - MAX 10

  String webpage;                      // Coverts Arduino elements to JSON elements
  String wsString = "";                      // Stores Websockets Script

  void tbClick(String _index, String _status);
  void tbuttonStatus();
  void handleWebpage(AsyncWebSocketClient * client);

 private:
  AsyncWebServer* server;       // Create Unique Instance for Webserver
  AsyncWebSocket* ws;
};

extern EasyUIClass EasyUI;
#endif
