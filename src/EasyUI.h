#ifndef EasyUI_h
#define EasyUI_h

#include "Arduino.h"
#include "stdlib_noniso.h"
#include "FS.h"
#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "ArduinoJson.h"
#include "WebSocketsServer.h"
#include "Hash.h"


#define HARDWARE "esp8266"

class EasyUIClass{

public:
  void begin();   // Begin HTTP Server + WebSocketsServer & Initalize All Elements
  void title(const char* _title);  // Define Webpage Header Name and title
  void toggleButton(uint8_t  pin, const char* tbutton_label, int start_state = 0, bool swap_state = false);   // Create Toggle Button
  void label(const char* label_name, const char*  label_val); // Create Label
  void loop();    // Do All Loop Work

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
  String ws = "";                      // Stores Websockets Script

  // Don't Issue the Below functions in your Sketch! - These are Resposible for Webpage functioning.
  void tbClick(String _index, String _status);
  void tbuttonStatus();
  void handleWebpage();


 private:
  std::unique_ptr<ESP8266WebServer> server;       // Create Unique Instance for Webserver
  std::unique_ptr<WebSocketsServer> webSocket;    // Create Unique Instance for WebSocketsServer


  void handleRoot();      // Handle Index HTML
  void handleNCSS();      // Handle Normalize CSS
  void handleSCSS();      // Handle Main Style CSS
  void handleJS();        // Handle JQuery
  void handleNotFound();  // Handle Page Not-Found
  void handleSockets();   // Handle Sockets Script


};

extern EasyUIClass EasyUI;
#endif
