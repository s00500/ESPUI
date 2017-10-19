#include "EasyUI.h"
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
              EasyUI.jsonDom(client);
              Serial.println("JSON Data Sent to Client!");
            }
            break;
        case WS_EVT_DATA:
          String msg = "";
          for (size_t i = 0; i < len; i++) {
            msg += (char) data[i];
          }
          if(msg.startsWith("bdown:")){
            EasyUI.controls[msg.substring(6).toInt()]->callback(msg.substring(6).toInt(), B_DOWN);
          }else if(msg.startsWith("bup:")){
            EasyUI.controls[msg.substring(4).toInt()]->callback(msg.substring(4).toInt(), B_UP);
          }else if(msg.startsWith("pfdown:")){
            EasyUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_FOR_DOWN);
          }else if(msg.startsWith("pfup:")){
            EasyUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_FOR_UP);
          }else if(msg.startsWith("pldown:")){
            EasyUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_LEFT_DOWN);
          }else if(msg.startsWith("plup:")){
            EasyUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_LEFT_UP);
          }else if(msg.startsWith("prdown:")){
            EasyUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_RIGHT_DOWN);
          }else if(msg.startsWith("prup:")){
            EasyUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_RIGHT_UP);
          }else if(msg.startsWith("pbdown:")){
            EasyUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_BACK_DOWN);
          }else if(msg.startsWith("pbup:")){
            EasyUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_BACK_UP);
          }else if(msg.startsWith("pcdown:")){
            EasyUI.controls[msg.substring(7).toInt()]->callback(msg.substring(7).toInt(), P_CENTER_DOWN);
          }else if(msg.startsWith("pcup:")){
            EasyUI.controls[msg.substring(5).toInt()]->callback(msg.substring(5).toInt(), P_CENTER_UP);
          }
          break;
    }
}

void EasyUIClass::label(const char* label){
  Control* newL = new Control();
  newL->type = UI_LABEL;
  newL->label = label;
  newL->oldValue = label;
  newL->callback = NULL;
  controls[cIndex] = newL;
  cIndex++;
}

void EasyUIClass::button(const char* label, void(* callBack)(int, int)){
  Control* newB = new Control();
  newB->type = UI_BUTTON;
  newB->label = label;
  newB->callback = callBack;
  controls[cIndex] = newB;
  cIndex++;
}

void EasyUIClass::switcher(const char* label, int start_state, void(* callBack)(int, int)){
  Control* newS = new Control();
  newS->type = UI_SWITCHER;
  newS->label = label;
  newS->callback = callBack;
  controls[cIndex] = newS;
  cIndex++;
  //TODO: implement switch state buffer
  //tbutton_swap[tb_index] = swap_state;
}

void EasyUIClass::pad(const char* label, bool center, void(* callBack)(int, int)){
  Control* newP = new Control();
  if(center)newP->type = UI_CPAD;
  else newP->type = UI_PAD;
  newP->label = label;
  newP->callback = callBack;
  controls[cIndex] = newP;
  cIndex++;
}

void EasyUIClass::print(int labelid, String value){
  if(labelid<cIndex && controls[labelid]->type == UI_LABEL){
    controls[labelid]->oldValue = value;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = UPDATE_LABEL;
    root["label"] = value;
    root["id"] = String(labelid);
    root.printTo(json);
    this->ws->textAll(json);
  }else{
    Serial.println(String("Error: ")+ String(labelid) +String(" is no label"));
  }
}


// Convert & Transfer Arduino elements to JSON elements
void EasyUIClass::jsonDom(AsyncWebSocketClient * client){
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
      root["id"] = String(i);
      if(controls[i]->type == UI_LABEL)root["label"] = controls[i]->oldValue;
    }
    root.printTo(json);
    client->text(json);
  }
}


void EasyUIClass::begin(const char * _title){
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

EasyUIClass EasyUI;
