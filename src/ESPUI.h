#ifndef ESPUI_h
#define ESPUI_h

#define debug true

#include "Arduino.h"
#include "ArduinoJson.h"
#include "FS.h"
#include "stdlib_noniso.h"

#if defined(ESP32)

#include "SPIFFS.h"
#include "WiFi.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#else

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>

#define FILE_WRITE "w"

#endif

typedef struct Control {
  unsigned int type;
  unsigned int id; // just mirroring the id here for practical reasons
  const char *label;
  void (*callback)(Control, int);
  String value;
  unsigned int color;
} Control;

// Types
#define UI_TITEL 0
#define UI_LABEL 1
#define UI_BUTTON 2
#define UI_SWITCHER 3
#define UI_PAD 4
#define UI_CPAD 5
#define UPDATE_LABEL 6
#define UPDATE_SWITCHER 7
#define UI_SLIDER 8
#define UPDATE_SLIDER 9

// Values
#define B_DOWN -1
#define B_UP 1

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

#define SL_VALUE 8


// Colors
#define COLOR_TURQUOISE 0
#define COLOR_EMERALD 1
#define COLOR_PETERRIVER 2
#define COLOR_WETASPHALT 3
#define COLOR_SUNFLOWER 4
#define COLOR_CARROT 5
#define COLOR_ALIZARIN 6
#define COLOR_NONE 6


class ESPUIClass {

public:
  void begin(const char *_title); // Setup servers and page

  void prepareFileSystem(); // Initially preps the filesystem and loads a lot of stuff into SPIFFS
void list();
  // Creating Elements
  void label(const char *label, int color, String value = ""); // Create Label
  void button(const char *label, void (*callBack)(Control, int), int color,
              String value = ""); // Create Event Button
  void switcher(const char *label, bool startState,
                void (*callBack)(Control, int),
                int color); // Create Toggle Button
  void pad(const char *label, bool centerButton, void (*callBack)(Control, int),
           int color); // Create Pad Control
  void slider(const char *label, void (*callBack)(Control, int), int color, String value); // Create Slider Control

  // Update Elements
  void print(int id, String value);
  void print(String label, String value);

  void updateSwitcher(int id, bool nValue, int clientId = -1);
  void updateSwitcher(String label, bool nValue, int clientId = -1);

  void updateSlider(int id, int nValue, int clientId = -1);
  void updateSlider(String label, int nValue, int clientId = -1);

  void textThem(String text, int clientId);

  // Variables ---
  const char *ui_title = "ESPUI"; // Store UI Title and Header Name
  int cIndex = 0;                 // Control index
  Control *controls[25];
  void jsonDom(AsyncWebSocketClient *client);
  int getIdByLabel(String label);
  bool labelExists(String label);

private:
  AsyncWebServer *server;
  AsyncWebSocket *ws;
};

extern ESPUIClass ESPUI;
#endif
