#include "ESPUI.h"

#include "dataIndexHTML.h"

#include "dataNormalizeCSS.h"
#include "dataStyleCSS.h"

#include "dataControlsJS.h"
#include "dataSliderJS.h"
#include "dataZeptoJS.h"

#include <ESPAsyncWebServer.h>
#include <functional>

// ################# Spiffs functions
#if defined(ESP32)
void listDir(const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = SPIFFS.open(dirname);

  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }

  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();

  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }

    file = root.openNextFile();
  }
}
#else

void listDir(const char *dirname, uint8_t levels) {
  // ignoring levels for esp8266
  Serial.printf("Listing directory: %s\n", dirname);

  String str = "";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.print("  FILE: ");
    Serial.print(dir.fileName());
    Serial.print("  SIZE: ");
    Serial.println(dir.fileSize());
  }
}

#endif

void ESPUIClass::list() {
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  listDir("/", 1);
#if defined(ESP32)
  Serial.println(SPIFFS.totalBytes());
  Serial.println(SPIFFS.usedBytes());
#else
  FSInfo fs_info;
  SPIFFS.info(fs_info);

  Serial.println(fs_info.totalBytes);
  Serial.println(fs_info.usedBytes);
#endif
}

void deleteFile(const char *path) {
  if (DEBUG_ESPUI) Serial.print(SPIFFS.exists(path));
  if (!SPIFFS.exists(path)) {
    Serial.printf("File: %s does not exist, not deleting\n", path);
    return;
  }

  Serial.printf("Deleting file: %s\n", path);

  if (SPIFFS.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void writeFile(const char *path, const char *data) {
  Serial.printf("Writing file: %s\n", path);

  File file = SPIFFS.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
#if defined(ESP32)
  if (file.print(data)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
#else
  if (file.print(FPSTR(data))) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
#endif
  file.close();
}

// end Spiffs functions

void ESPUIClass::prepareFileSystem() {
  // this function should only be used once

  Serial.println("About to prepare filesystem...");

#if defined(ESP32)
  SPIFFS.format();
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  listDir("/", 1);
  Serial.println("SPIFFS Mount ESP32 Done");
#else
  SPIFFS.format();
  SPIFFS.begin();
  Serial.println("SPIFFS Mount ESP8266 Done");
#endif

  deleteFile("/index.htm");

  deleteFile("/css/style.css");
  deleteFile("/css/normalize.css");

  deleteFile("/js/zepto.min.js");
  deleteFile("/js/controls.js");
  deleteFile("/js/slider.js");

  Serial.println("Cleanup done");

  // Now write
  writeFile("/index.htm", HTML_INDEX);

  writeFile("/css/style.css", CSS_STYLE);
  writeFile("/css/normalize.css", CSS_NORMALIZE);

  writeFile("/js/zepto.min.js", JS_ZEPTO);
  writeFile("/js/controls.js", JS_CONTROLS);
  writeFile("/js/slider.js", JS_SLIDER);

  Serial.println("Done Initializing filesystem :-)");

#if defined(ESP32)
  if (DEBUG_ESPUI) listDir("/", 1);
#endif

  SPIFFS.end();
}

// Handle Websockets Communication
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_DISCONNECT: {
      if (DEBUG_ESPUI) Serial.printf("Disconnected!\n");
      break;
    }
    case WS_EVT_PONG: {
      if (DEBUG_ESPUI) Serial.printf("Received PONG!\n");
      break;
    }
    case WS_EVT_ERROR: {
      if (DEBUG_ESPUI) Serial.printf("WebSocket Error!\n");
      break;
    }
    case WS_EVT_CONNECT: {
      if (DEBUG_ESPUI) {
        Serial.print("Connected: ");
        Serial.println(client->id());
      }

      ESPUI.jsonDom(client);
      if (DEBUG_ESPUI) {
        Serial.println("JSON Data Sent to Client!");
      }
    } break;
    case WS_EVT_DATA: {
      String msg = "";
      for (size_t i = 0; i < len; i++) {
        msg += (char)data[i];
      }

      int id = msg.substring(msg.lastIndexOf(':') + 1).toInt();
      if (id >= ESPUI.cIndex) {
        if (DEBUG_ESPUI) Serial.println("Maleformated id in websocket message");
        return;
      }

      Control *c =
          ESPUI.controls[msg.substring(msg.lastIndexOf(':') + 1).toInt()];

      if (msg.startsWith("bdown:")) {
        c->callback(*c, B_DOWN);
      } else if (msg.startsWith("bup:")) {
        c->callback(*c, B_UP);
      } else if (msg.startsWith("pfdown:")) {
        c->callback(*c, P_FOR_DOWN);
      } else if (msg.startsWith("pfup:")) {
        c->callback(*c, P_FOR_UP);
      } else if (msg.startsWith("pldown:")) {
        c->callback(*c, P_LEFT_DOWN);
      } else if (msg.startsWith("plup:")) {
        c->callback(*c, P_LEFT_UP);
      } else if (msg.startsWith("prdown:")) {
        c->callback(*c, P_RIGHT_DOWN);
      } else if (msg.startsWith("prup:")) {
        c->callback(*c, P_RIGHT_UP);
      } else if (msg.startsWith("pbdown:")) {
        c->callback(*c, P_BACK_DOWN);
      } else if (msg.startsWith("pbup:")) {
        c->callback(*c, P_BACK_UP);
      } else if (msg.startsWith("pcdown:")) {
        c->callback(*c, P_CENTER_DOWN);
      } else if (msg.startsWith("pcup:")) {
        c->callback(*c, P_CENTER_UP);
      } else if (msg.startsWith("sactive:")) {
        ESPUI.updateSwitcher(c->id, true);
        c->callback(*c, S_ACTIVE);
      } else if (msg.startsWith("sinactive:")) {
        ESPUI.updateSwitcher(c->id, false);
        c->callback(*c, S_INACTIVE);
      } else if (msg.startsWith("slvalue:")) {
        int value =
            msg.substring(msg.indexOf(':') + 1, msg.lastIndexOf(':')).toInt();
        ESPUI.updateSlider(c->id, value, client->id());
        c->callback(*c, SL_VALUE);
      } else if (msg.startsWith("nvalue:")) {
        int value =
            msg.substring(msg.indexOf(':') + 1, msg.lastIndexOf(':')).toInt();
        ESPUI.updateNumber(c->id, value, client->id());
        c->callback(*c, N_VALUE);
      } else if (msg.startsWith("tvalue:")) {
        String value =
            msg.substring(msg.indexOf(':') + 1, msg.lastIndexOf(':'));
        ESPUI.updateText(c->id, value, client->id());
        c->callback(*c, T_VALUE);
      }
    } break;
    default:
      break;
  }
}

int ESPUIClass::label(const char *label, int color, String value) {
  if (labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element " + String(label) +
                     " exists, skipping creating element!");
    return -1;
  }

  Control *newL = new Control();
  newL->type = UI_LABEL;
  newL->label = label;
  newL->color = color;
  if (value != "")
    newL->value = value;  // Init with labeltext
  else
    newL->value = String(label);
  newL->callback = NULL;
  newL->id = cIndex;
  controls[cIndex] = newL;
  cIndex++;
  return cIndex - 1;
}

int ESPUIClass::graph(const char *label, int color) {
  if (labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element " + String(label) +
                     " exists, skipping creating element!");
    return -1;
  }

  Control *newG = new Control();
  newG->type = UI_GRAPH;
  newG->label = label;
  newG->color = color;
  newG->id = cIndex;
  controls[cIndex] = newG;
  cIndex++;
  return cIndex - 1;
}

// TODO: this still needs a range setting
int ESPUIClass::slider(const char *label, void (*callBack)(Control, int),
                       int color, String value) {
  if (labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element " + String(label) +
                     " exists, skipping creating element!");
    return -1;
  }

  Control *newSL = new Control();
  newSL->type = UI_SLIDER;
  newSL->label = label;
  newSL->color = color;
  if (value != "")
    newSL->value = value;
  else
    newSL->value = "";  // TODO: init with half value
  newSL->callback = callBack;
  newSL->id = cIndex;
  controls[cIndex] = newSL;
  cIndex++;
  return cIndex - 1;
}

int ESPUIClass::button(const char *label, void (*callBack)(Control, int),
                       int color, String value) {
  if (labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element " + String(label) +
                     " exists, skipping creating element!");
    return -1;
  }

  Control *newB = new Control();
  newB->type = UI_BUTTON;
  newB->label = label;
  newB->color = color;

  if (value != "")
    newB->value = value;  // Init with labeltext
  else
    newB->value = String(label);

  newB->callback = callBack;
  newB->id = cIndex;
  controls[cIndex] = newB;
  cIndex++;
  return cIndex - 1;
}

int ESPUIClass::switcher(const char *label, bool startState,
                         void (*callBack)(Control, int), int color) {
  if (labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element " + String(label) +
                     " exists, skipping creating element!");
    return -1;
  }

  Control *newS = new Control();
  newS->type = UI_SWITCHER;
  newS->label = label;
  newS->color = color;
  newS->value = String(startState);
  newS->callback = callBack;
  newS->id = cIndex;
  controls[cIndex] = newS;
  cIndex++;
  return cIndex - 1;
}

int ESPUIClass::pad(const char *label, bool center,
                    void (*callBack)(Control, int), int color) {
  if (labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element " + String(label) +
                     " exists, skipping creating element!");
    return -1;
  }

  Control *newP = new Control();
  if (center)
    newP->type = UI_CPAD;
  else
    newP->type = UI_PAD;
  newP->label = label;
  newP->color = color;
  newP->callback = callBack;
  newP->id = cIndex;
  controls[cIndex] = newP;
  cIndex++;
  return cIndex - 1;
}

// TODO: min and max need to be saved, they also need to be sent to the frontend
int ESPUIClass::number(const char *label, void (*callBack)(Control, int),
                       int color, int number, int min, int max) {
  if (labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element " + String(label) +
                     " exists, skipping creating element!");
    return -1;
  }

  Control *newN = new Control();
  newN->type = UI_NUMBER;
  newN->label = label;
  newN->color = color;
  newN->value = String(number);
  newN->callback = callBack;
  newN->id = cIndex;
  controls[cIndex] = newN;
  cIndex++;
  return cIndex - 1;
}

int ESPUIClass::text(const char *label, void (*callBack)(Control, int),
                     int color, String value) {
  if (labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element " + String(label) +
                     " exists, skipping creating element!");
    return -1;
  }

  Control *newT = new Control();
  newT->type = UI_TEXT_INPUT;
  newT->label = label;
  newT->color = color;
  newT->value = value;
  newT->callback = callBack;
  newT->id = cIndex;
  controls[cIndex] = newT;
  cIndex++;
  return cIndex - 1;
}

void ESPUIClass::print(int id, String value) {
  if (id < cIndex && controls[id]->type == UI_LABEL) {
    controls[id]->value = value;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["type"] = UPDATE_LABEL;
    root["value"] = value;
    root["id"] = String(id);
    root.printTo(json);
    this->ws->textAll(json);
  } else {
    if (DEBUG_ESPUI)
      Serial.println(String("Error: ") + String(id) + String(" is no label"));
  }
}

void ESPUIClass::print(String label, String value) {
  if (!labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element does not " + String(label) +
                     " exist, cannot update!");
    return;
  }
  print(getIdByLabel(label), value);
}

void ESPUIClass::updateSlider(int id, int nValue, int clientId) {
  if (id < cIndex && controls[id]->type == UI_SLIDER) {
    controls[id]->value = nValue;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["type"] = UPDATE_SLIDER;
    root["value"] = nValue;
    root["id"] = String(id);
    root.printTo(json);
    textThem(json, clientId);
  } else {
    if (DEBUG_ESPUI)
      Serial.println(String("Error: ") + String(id) + String(" is no slider"));
  }
}

void ESPUIClass::updateSwitcher(int id, bool nValue, int clientId) {
  if (id < cIndex && controls[id]->type == UI_SWITCHER) {
    controls[id]->value = nValue ? 1 : 0;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["type"] = UPDATE_SWITCHER;
    root["value"] = nValue ? 1 : 0;
    root["id"] = String(id);
    root.printTo(json);
    textThem(json, clientId);
  } else {
    if (DEBUG_ESPUI)
      Serial.println(String("Error: ") + String(id) +
                     String(" is no switcher"));
  }
}

void ESPUIClass::updateSwitcher(String label, bool nValue, int clientId) {
  if (!labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element does not " + String(label) +
                     " exist, cannot update!");
    return;
  }
  updateSwitcher(getIdByLabel(label), nValue, clientId);
}

void ESPUIClass::updateNumber(int id, int number, int clientId) {
  if (id < cIndex && controls[id]->type == UI_NUMBER) {
    controls[id]->value = number;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["type"] = UPDATE_NUMBER;
    root["value"] = String(number);
    root["id"] = String(id);
    root.printTo(json);
    textThem(json, clientId);
  } else {
    if (DEBUG_ESPUI)
      Serial.println(String("Error: ") + String(id) + String(" is no number"));
  }
}

void ESPUIClass::updateNumber(String label, int number, int clientId) {
  if (!labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element does not " + String(label) +
                     " exist, cannot update!");
    return;
  }
  updateNumber(getIdByLabel(label), number, clientId);
}

void ESPUIClass::updateText(int id, String text, int clientId) {
  if (id < cIndex && controls[id]->type == UI_TEXT_INPUT) {
    controls[id]->value = text;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["type"] = UPDATE_TEXT_INPUT;
    root["value"] = String(text);
    root["id"] = String(id);
    root.printTo(json);
    textThem(json, clientId);
  } else {
    if (DEBUG_ESPUI)
      Serial.println(String("Error: ") + String(id) + String(" is no number"));
  }
}

void ESPUIClass::updateText(String label, String text, int clientId) {
  if (!labelExists(label)) {
    if (DEBUG_ESPUI)
      Serial.println("UI ERROR: Element does not " + String(label) +
                     " exist, cannot update!");
    return;
  }
  updateText(getIdByLabel(label), text, clientId);
}

// This is a hacky workaround because ESPAsyncWebServer does not have a function
// like this and it's clients array is private
void ESPUIClass::textThem(String text, int clientId) {
  int tryId = 0;
  for (int count = 0; count < this->ws->count();) {
    if (this->ws->hasClient(tryId)) {
      if (clientId != tryId) {
        this->ws->client(tryId)->text(text);
      }
      count++;
    }
    tryId++;
  }
}

int ESPUIClass::getIdByLabel(String label) {
  for (int i = 0; i < cIndex; i++) {
    if (String(controls[i]->label) == label) return i;
  }
  return -1;  // failed, nonexistant
}

bool ESPUIClass::labelExists(String label) {
  for (int i = 0; i < cIndex; i++) {
    if (String(controls[i]->label) == label) return true;
  }
  return false;
}

// Convert & Transfer Arduino elements to JSON elements
void ESPUIClass::jsonDom(AsyncWebSocketClient *client) {
  for (int i = -1; i < cIndex; i++) {
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    if (i == -1) {
      root["type"] = UI_TITEL;
      root["label"] = String(ui_title);
    } else {
      root["type"] = controls[i]->type;
      root["label"] = String(controls[i]->label);
      root["value"] = String(controls[i]->value);
      root["color"] = String(controls[i]->color);
      root["id"] = String(i);
    }
    root.printTo(json);
    client->text(json);
  }
}

void ESPUIClass::beginSPIFFS(const char *_title) {
  ui_title = _title;
  server = new AsyncWebServer(80);
  ws = new AsyncWebSocket("/ws");

  if (!SPIFFS.begin()) {
    Serial.println(
        "SPIFFS Mount Failed, PLEASE CHECK THE README ON HOW TO "
        "PREPARE YOUR ESP!!!!!!!");
    return;
  }
  listDir("/", 1);

  if (!SPIFFS.exists("/index.htm")) {
    Serial.println(
        "Please read the README!!!!!!!, Make sure to "
        "ESPUI.prepareFileSystem() once in an empty sketch");
    return;
  }

  ws->onEvent(onWsEvent);
  server->addHandler(ws);
  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");

  // Heap for general Servertest
  server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain",
                  String(ESP.getFreeHeap()) + " In SPIFFSmode");
  });

  server->onNotFound(
      [](AsyncWebServerRequest *request) { request->send(404); });

  server->on("/zepto.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(
        200, "application/javascript", JS_ZEPTO_GZIP, sizeof(JS_ZEPTO_GZIP));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  server->begin();
  if (DEBUG_ESPUI) Serial.println("UI Initialized");
}

void ESPUIClass::begin(const char *_title) {
  ui_title = _title;
  server = new AsyncWebServer(80);
  ws = new AsyncWebSocket("/ws");

  ws->onEvent(onWsEvent);
  server->addHandler(ws);

  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response =
        request->beginResponse_P(200, "text/html", HTML_INDEX);
    request->send(response);
  });

  // Javascript files

  server->on("/js/zepto.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(
        200, "application/javascript", JS_ZEPTO_GZIP, sizeof(JS_ZEPTO_GZIP));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  server->on("/js/controls.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response =
        request->beginResponse_P(200, "application/javascript",
                                 JS_CONTROLS_GZIP, sizeof(JS_CONTROLS_GZIP));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  server->on("/js/slider.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(
        200, "application/javascript", JS_SLIDER_GZIP, sizeof(JS_SLIDER_GZIP));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  // Stylesheets

  server->on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(
        200, "text/css", CSS_STYLE_GZIP, sizeof(CSS_STYLE_GZIP));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  server->on(
      "/css/normalize.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(
            200, "text/css", CSS_NORMALIZE_GZIP, sizeof(CSS_NORMALIZE_GZIP));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
      });

  // Heap for general Servertest
  server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain",
                  String(ESP.getFreeHeap()) + " In Memorymode");
  });

  server->onNotFound(
      [](AsyncWebServerRequest *request) { request->send(404); });

  server->begin();
  if (DEBUG_ESPUI) Serial.println("UI Initialized");
}

ESPUIClass ESPUI;
