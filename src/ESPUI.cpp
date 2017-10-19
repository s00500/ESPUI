#include "ESPUI.h"
#include <ESPAsyncWebServer.h>
#include <functional>

// Handle Websockets Communication
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
    switch(type) {
        case WS_EVT_DISCONNECT:
            Serial.printf("Disconnected!\n");
            break;
        case WS_EVT_CONNECT:
            {
				      Serial.println("Connected");
              ESPUI.jsonDom(client);
              Serial.println("JSON Data Sent to Client!");
            }
            break;
        case WS_EVT_DATA:
          String msg = "";
          for (size_t i = 0; i < len; i++) {
            msg += (char) data[i];
          }
          if(msg.startsWith("bdown:")){
            ESPUI.controls[msg.substring(6).toInt()]->callback(msg.substring(6).toInt(), B_DOWN);
          }else if(msg.startsWith("bup:")){
            ESPUI.controls[msg.substring(4).toInt()]->callback(msg.substring(4).toInt(), B_UP);
          }else if(msg.startsWith("pfdown:")){
            ESPUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_FOR_DOWN);
          }else if(msg.startsWith("pfup:")){
            ESPUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_FOR_UP);
          }else if(msg.startsWith("pldown:")){
            ESPUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_LEFT_DOWN);
          }else if(msg.startsWith("plup:")){
            ESPUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_LEFT_UP);
          }else if(msg.startsWith("prdown:")){
            ESPUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_RIGHT_DOWN);
          }else if(msg.startsWith("prup:")){
            ESPUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_RIGHT_UP);
          }else if(msg.startsWith("pbdown:")){
            ESPUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_BACK_DOWN);
          }else if(msg.startsWith("pbup:")){
            ESPUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_BACK_UP);
          }else if(msg.startsWith("pcdown:")){
            ESPUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_CENTER_DOWN);
          }else if(msg.startsWith("pcup:")){
            ESPUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_CENTER_UP);
          }else if(msg.startsWith("sactive:")){
            ESPUI.updateSwitcher(msg.substring(8).toInt(), true);
            ESPUI.controls[msg.substring(8).toInt()]->callback(msg.substring(8).toInt(), S_ACTIVE);
          }else if(msg.startsWith("sinactive:")){
            ESPUI.updateSwitcher(msg.substring(10).toInt(), false);
            ESPUI.controls[msg.substring(10).toInt()]->callback(msg.substring(10).toInt(), S_INACTIVE);
          }
          break;
    }
}

void ESPUIClass::label(const char* label, String value){
  Control* newL = new Control();
  newL->type = UI_LABEL;
  newL->label = label;
  if(value != "") newL->value = value; // Init with labeltext
  else newL->value = String(label);
  newL->callback = NULL;
  controls[cIndex] = newL;
  cIndex++;
}

void ESPUIClass::button(const char* label, void(* callBack)(int, int)){
  Control* newB = new Control();
  newB->type = UI_BUTTON;
  newB->label = label;
  newB->callback = callBack;
  controls[cIndex] = newB;
  cIndex++;
}

void ESPUIClass::switcher(const char* label, bool startState, void(* callBack)(int, int)){
  Control* newS = new Control();
  newS->type = UI_SWITCHER;
  newS->label = label;
  newS->value = String(startState);
  newS->callback = callBack;
  controls[cIndex] = newS;
  cIndex++;
}

void ESPUIClass::pad(const char* label, bool center, void(* callBack)(int, int)){
  Control* newP = new Control();
  if(center)newP->type = UI_CPAD;
  else newP->type = UI_PAD;
  newP->label = label;
  newP->callback = callBack;
  controls[cIndex] = newP;
  cIndex++;
}

void ESPUIClass::print(int id, String value){
  if(id<cIndex && controls[id]->type == UI_LABEL){
    controls[id]->value = value;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = UPDATE_LABEL;
    root["value"] = value;
    root["id"] = String(id);
    root.printTo(json);
    this->ws->textAll(json);
  }else{
    Serial.println(String("Error: ")+ String(id) +String(" is no label"));
  }
}

void ESPUIClass::updateSwitcher(int id, bool nValue){
  if(id<cIndex && controls[id]->type == UI_SWITCHER){
    controls[id]->value = nValue?1:0;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = UPDATE_SWITCH;
    root["value"] = nValue?1:0;
    root["id"] = String(id);
    root.printTo(json);
    this->ws->textAll(json);
  }else{
    Serial.println(String("Error: ")+ String(id) +String(" is no switcher"));
  }
}


// Convert & Transfer Arduino elements to JSON elements
void ESPUIClass::jsonDom(AsyncWebSocketClient * client){
  for(int i=-1; i<cIndex; i++){
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    if(i == -1){
      root["type"] = UI_TITEL;
      root["label"] = String(ui_title);
    }else{
      root["type"] = controls[i]->type;
      root["label"] = String(controls[i]->label);
      root["value"] = String(controls[i]->value);
      root["id"] = String(i);
    }
    root.printTo(json);
    client->text(json);
  }
}


void ESPUIClass::begin(const char * _title){
  ui_title = _title;
  server = new AsyncWebServer(80);
  ws = new AsyncWebSocket("/ws");
  SPIFFS.begin();
  ws->onEvent(onWsEvent);
  server->addHandler(ws);
  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");

  //Heap for general Servertest
  server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server->onNotFound([](AsyncWebServerRequest *request){
    request->send(404);
  });

  server->begin();
  Serial.println("UI Initialized");
}

ESPUIClass ESPUI;
