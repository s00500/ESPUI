#include "EasyUI.h"

#include "HTML_PAGE.h"      // Added HTML Index Page
#include "CSS_STYLE.h"      // Added Main CSS Style
#include "CSS_NORMALIZE.h"  // Added Normalize CSS
#include "JS_ZEPTO.h"
#include <ESPAsyncWebServer.h>

#include <functional>
using namespace std;
using namespace std::placeholders;

/*
// Generate Websockets Script for Webpage
void EasyUIClass::handleSockets(AsyncWebServerRequest *request){
  String ip;
  if(WiFi.localIP().toString() != "0.0.0.0"){
   ip = WiFi.localIP().toString();
  }
  else if(WiFi.softAPIP().toString() != "0.0.0.0"){
   ip = WiFi.softAPIP().toString();
  }

  String wsString;
  wsString = "var connection = new WebSocket(\"ws://"+ip+":81/\", ['easyui']);";
  wsString += " var keys = {};";
  wsString += " connection.onopen = function () {";
  wsString += " $(\"#connection_status\").toggleClass(\"color-green\");";
  wsString += " $(\"#connection_status\").text(\"Connected\");";
  wsString += String(" connection.send(")+"\"{'mode': 'check_tb_status'}\");";
  for(int i=0; i<tb_index; i++){
    wsString += String(" $(document).on('click',")+"'#tb"+i+"', function() {";
    wsString += String(" if($('#tb")+i+"').hasClass(\"button\")){ console.log(\"Button Clicked!\"); connection.send("+"\"{'mode': 'tb_click', 'index': '"+i+"', 'status': 'on'}\""+"); }";
    wsString += String(" else if($('#tb")+i+"').hasClass(\"button-primary\")){ connection.send("+"\"{'mode': 'tb_click', 'index': '"+i+"', 'status': 'off'}\""+"); } });";
  }
  wsString += " };";
  wsString += " connection.onerror = function (error) { $('#connection_status').toggleClass(\"color-red\"); $(\"#connection_status\").text(\"Error / No Connection\"); };";
  wsString += " connection.onmessage = function (e) {";
  wsString += " console.log(e.data); ";
  wsString += " var obj = jQuery.parseJSON(e.data);";
  wsString += " if(obj.mode === 'create_label'){ $('#row').append(\"<div class='two columns card'><h5>\"+obj.l_title+\"</h5><hr /><h3><span class='label'>\"+obj.l_value+\"</span></h3></div>\");}";
  wsString += " else if(obj.mode === 'create_tbutton'){ $('#row').append(\"<div class='two columns card'><h5>\"+obj.tb_title+\"</h5><hr/><button id=\"+\"tb\"+obj.index+\"></button></div>\");}";
  wsString += " else if(obj.mode === 't_button_startup'){ var tb_index = obj.index; for(i=0; i<tb_index; i++){ var tb_index2 = \"tb\"+i; var tb_status = obj[tb_index2]; var tb_index3 = \"#\"+tb_index2;  console.log(tb_status); if(tb_status === \"1\"){$(tb_index3).toggleClass('button-primary'); $(tb_index3).text('Turn Off');} else if(tb_status === \"0\"){$(tb_index3).toggleClass('button'); $(tb_index3).text('Turn On');} } }";
  wsString += " else if(obj.mode === 't_button_click'){ var tb_index = \"tb\"+obj.index; var tb_status = obj[tb_index]; var tb_index3 = \"#\"+tb_index;  console.log(tb_status); if(tb_status == \"1\"){ console.log(\"Status Tuned ON\");  $(tb_index3).text('Turn Off'); $(tb_index3).removeClass( \"button\" ).addClass( \"button-primary\" );} else if(tb_status == \"0\"){console.log(\"Status Tuned OFF\"); $(tb_index3).text('Turn On'); $(tb_index3).removeClass( \"button-primary\" ).addClass( \"button\" ); } } };";
  request->send(200, "application/javascript", wsString);
}

// Handle Index HTML
void EasyUIClass::handleRoot(AsyncWebServerRequest *request){
  String rootString;
  //request->setContentLength(CONTENT_LENGTH_UNKNOWN);
  rootString += HTML_HEAD1;
  rootString += String("<title>")+ui_title+"</title>";
  rootString += HTML_HEAD2;
  rootString += String("<h4>")+ui_title+"  <span id=\"connection_status\" class=\"label\">Offline</span></h4></div><hr />";
  rootString += HTML_BODY;
  rootString += "<script src=\"/js/zepto.js\"></script>";

  rootString += HTML_END;
  request->send(200,"text/html", rootString);
  //server->client().stop();
}
*/
/*
// Handle Websockets Communication
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
    switch(type) {
        case WS_EVT_DISCONNECT:
            Serial.printf("Disconnected!\n");
            break;
        case WS_EVT_CONNECT:
            {
              //Serial.printf("[WSc] Connected to url: %s\n",  payload);
				      Serial.println("Connected");
              EasyUI.handleWebpage();
              Serial.println("JSON Data Sent to Client!");
            }
            break;
        case WS_EVT_DATA:
            StaticJsonBuffer<200> jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(data);
             String mode = root["mode"];
             if(mode == "check_tb_status"){
               EasyUI.tbuttonStatus();
             }
             else if(mode == "tb_click"){
               String status = root["status"];
               String index = root["index"];
              EasyUI.tbClick(index, status);
             }
            break;
    }
}
*/
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
void EasyUIClass::button(uint8_t  pin, const char* tbutton_label, int start_state, bool swap_state){
//TODO: Implement
  pinMode(pin, OUTPUT);
  digitalWrite(pin, start_state);
  tbutton_swap[tb_index] = swap_state;
  tbutton_pinout[tb_index] = pin;
  tbuttontitle[tb_index] = tbutton_label;
  tb_index++;

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
/*
// Convert & Transfer Arduino elements to JSON elements
void EasyUIClass::handleWebpage(){

  for(int i=0; i<l_index; i++){
    String json;
    StaticJsonBuffer<200> jsonBuffer1;
    JsonObject& root1 = jsonBuffer1.createObject();
    root1["mode"] = "create_label";
    root1["l_title"] = String(label_title[i]);
    root1["l_value"] = String(label_value[i]);
    root1.printTo(json);
    webSocket->broadcastTXT(json);
  }

  for(int i=0; i<tb_index; i++){
    String json;
    StaticJsonBuffer<200> jsonBuffer2;
    JsonObject& root2 = jsonBuffer2.createObject();
    root2["mode"] = "create_tbutton";
    root2["index"] = String(i);
    root2["tb_title"] = tbuttontitle[i];
    root2.printTo(json);
    webSocket->broadcastTXT(json);
    }

}
*/

void EasyUIClass::begin(){
    server.reset(new AsyncWebServer(80));
    //ws.reset(new AsyncWebSocket("/ws"));
    //SPIFFS.begin();

    //ws->onEvent(onWsEvent);
    //server->addHandler(&ws);
/*
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    EasyUI.handleRoot(request);
  });
  server->on("/js/sockets.js", HTTP_GET, [](AsyncWebServerRequest *request){
    EasyUI.handleSockets(request);
  });
*/
  // Static constant serving =P
  server->on("/js/zepto.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/javascript", JS_ZEPTO);
  });
  server->on("/css/normalize.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/css", CSS_NORMALIZE);
  });
  server->on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/css", CSS_STYLE);
  });
  //Heap for general Servertest
  server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server->onNotFound([](AsyncWebServerRequest *request){
    request->send(404);
  });

  server->begin();
  //webSocket->onEvent(webSocketEvent);

  Serial.println("UI Initialized");
}

void EasyUIClass::loop(){
}

EasyUIClass EasyUI;
