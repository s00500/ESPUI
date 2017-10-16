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
          Serial.print("WS Event");
          String msg = "";
          for (size_t i = 0; i < len; i++) {
            msg += (char) data[i];
          }
          Serial.println(msg);
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& root = jsonBuffer.parseObject(msg);
             String type = root["type"];
             if(type == "t"){
               //Button Action
               //TODO: get Button here
               EasyUI.buttons[0]->callback();
               //EasyUI.tbuttonStatus();
             }
             /*
             else if(mode == "tb_click"){
               String status = root["status"];
               String index = root["index"];
              //EasyUI.tbClick(index, status);
             }
             */
            break;
    }
}

void EasyUIClass::title(const char* _title){
  ui_title = _title;
}

// Create Labels
void EasyUIClass::label(const char* label_name,const char*  label_val){
  label_value[l_index] = label_val;
  label_title[l_index] = label_name;
  l_index++;
}
/*
// Create Toggle Buttons
void EasyUIClass::toggleButton(uint8_t  pin, const char* tbutton_label, int start_state, bool swap_state){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, start_state);
  tbutton_swap[tb_index] = swap_state;
  tbutton_pinout[tb_index] = pin;
  tbuttontitle[tb_index] = tbutton_label;
  tb_index++;
}
*/
// Create a generic Button
void EasyUIClass::button(const char* bLabel, void(* callBack)()){

//TODO: Implement
  Button* newB = new Button();

  newB->label = bLabel;
  newB->callback = callBack;
  buttons[bIndex] = newB;
  bIndex++;
}
/*
// Check Toggle Buttons States and Transfer to Webpage
void EasyUIClass::tbuttonStatus(){
  String json;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["mode"] = "t_button_startup";
  root["index"] = tb_index;
    for(int i=0; i<tb_index; i++){
      String name = "tb"+String(i);
      int stat = digitalRead(tbutton_pinout[i]);
    if(tbutton_swap[i]){
      if(stat == HIGH)
      root[name] = "0";
     else if(stat == LOW)
      root[name] = "1";
     else
     root[name] = "unknown";
   }
   else{
    if(stat == HIGH)
     root[name] = "1";
    else if(stat == LOW)
     root[name] = "0";
    else
    root[name] = "unknown";
   }
}
    root.printTo(json);
  webSocket->broadcastTXT(json);
}
*/

/*
// Handle Toggle Button Click Response
void EasyUIClass::tbClick(String _index, String _status){
  String json;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  String name = "tb"+_index;
  root["mode"] = "t_button_click";
  root["index"] = _index;

  if(_status == "on"){
    root[name] = "1";
    root.printTo(json);
    webSocket->broadcastTXT(json);
    if(tbutton_swap[_index.toInt()]){
      digitalWrite(tbutton_pinout[_index.toInt()], LOW);
    }else{
      digitalWrite(tbutton_pinout[_index.toInt()], HIGH);
    }
  }
  else if(_status == "off"){
    root[name] = "0";
    root.printTo(json);
    webSocket->broadcastTXT(json);
  if(tbutton_swap[_index.toInt()]){
    digitalWrite(tbutton_pinout[_index.toInt()], HIGH);
  }else{
    digitalWrite(tbutton_pinout[_index.toInt()], LOW);
  }
  }
}
*/

// Convert & Transfer Arduino elements to JSON elements
void EasyUIClass::jsonDom(AsyncWebSocketClient * client){
  //SiteTitle
  //TODO: emit here
  // Labels
  for(int i=0; i<l_index; i++){
    String json;
    StaticJsonBuffer<200> jsonBuffer1;
    JsonObject& root1 = jsonBuffer1.createObject();
    root1["type"] = "domLabel";
    root1["l_title"] = String(label_title[i]);
    root1["label"] = String(label_value[i]);
    root1.printTo(json);
    client->text(json);
  }
  //  Buttons
  for(int i=0; i<bIndex; i++){
    String json;
    StaticJsonBuffer<200> jsonBuffer2;
    JsonObject& root2 = jsonBuffer2.createObject();
    root2["type"] = "domButton";
    root2["index"] = String(i);
    root2["label"] = buttons[i]->label;
    root2.printTo(json);
    client->text(json);
    }

}


void EasyUIClass::begin(){
  server = new AsyncWebServer(80);
  ws = new AsyncWebSocket("/ws");
  SPIFFS.begin(false);

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
