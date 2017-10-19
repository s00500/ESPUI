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

typedef struct Control
{
    unsigned int type;
    const char *label;
    void (*callback)(int, int);
    String value;
} Control;

// Types
#define UI_TITEL 0
#define UI_LABEL 1
#define UI_BUTTON 2
#define UI_SWITCHER 3
#define UI_PAD 4
#define UI_CPAD 5
#define UPDATE_LABEL 6
#define UPDATE_SWITCH 7

// Values
#define B_DOWN -1
#define B_UP  1

#define P_LEFT_DOWN -2
#define P_LEFT_UP 2
#define P_RIGHT_DOWN -3
#define P_RIGHT_UP 3
#define P_FOR_DOWN -4
#define P_FOR_UP 4
#define P_BACK_DOWN -5
#define P_BACK_UP 5
#define P_CENTER_DOWN -6
#define P_CENTER_UP 6
#define S_ACTIVE -7
#define S_INACTIVE 7



class EasyUIClass{

public:
  void begin(const char* _title);   // Setup servers and page

  // Creating Elements
  void label(const char* label, String value = ""); // Create Label
  void button(const char* label, void(* callBack)(int, int));   // Create Event Button
  void switcher(const char* label, bool startState, void(* callBack)(int, int));   // Create Toggle Button
  void pad(const char* label, bool centerButton, void(* callBack)(int, int));   // Create Pad Control

  // Update Elements
  void print(int id, String value);
  void updateSwitcher(int id, bool nValue);

  // Variables ---
  const char* ui_title = "ESPUI"; // Store UI Title and Header Name
  int cIndex;   // Control index
  Control* controls[25];
  void jsonDom(AsyncWebSocketClient * client);

private:
  AsyncWebServer* server;
  AsyncWebSocket* ws;
};

extern EasyUIClass EasyUI;
#endif
