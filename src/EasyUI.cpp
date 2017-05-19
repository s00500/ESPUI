#include "EasyUI.h"

#include "HTML_PAGE.h"      // Added HTML Index Page
#include "CSS_STYLE.h"      // Added Main CSS Style
#include "CSS_NORMALIZE.h"  // Added Normalize CSS
#include "JS_JQUERY.h"

#include <functional>
using namespace std;
using namespace std::placeholders;


// Generate Websockets Script for Webpage
void EasyUIClass::handleSockets(){
  String ip;
  if(WiFi.localIP().toString() != "0.0.0.0"){
   ip = WiFi.localIP().toString();
  }
  else if(WiFi.softAPIP().toString() != "0.0.0.0"){
   ip = WiFi.softAPIP().toString();
  }

  String ws;
  ws = "var connection = new WebSocket(\"ws://"+ip+":81/\", ['easyui']);";
  ws += " var keys = {};";
  ws += " connection.onopen = function () {";
  ws += " $(\"#connection_status\").toggleClass(\"color-green\");";
  ws += " $(\"#connection_status\").text(\"Connected\");";
  ws += String(" connection.send(")+"\"{'mode': 'check_tb_status'}\");";
  for(int i=0; i<tb_index; i++){
    ws += String(" $(document).on('click',")+"'#tb"+i+"', function() {";
    ws += String(" if($('#tb")+i+"').hasClass(\"button\")){ console.log(\"Button Clicked!\"); connection.send("+"\"{'mode': 'tb_click', 'index': '"+i+"', 'status': 'on'}\""+"); }";
    ws += String(" else if($('#tb")+i+"').hasClass(\"button-primary\")){ connection.send("+"\"{'mode': 'tb_click', 'index': '"+i+"', 'status': 'off'}\""+"); } });";
  }
  ws += " };";
  ws += " connection.onerror = function (error) { $('#connection_status').toggleClass(\"color-red\"); $(\"#connection_status\").text(\"Error / No Connection\"); };";
  ws += " connection.onmessage = function (e) {";
  ws += " console.log(e.data); ";
  ws += " var obj = jQuery.parseJSON(e.data);";
  ws += " if(obj.mode === 'create_label'){ $('#row').append(\"<div class='two columns card'><h5>\"+obj.l_title+\"</h5><hr /><h3><span class='label'>\"+obj.l_value+\"</span></h3></div>\");}";
  ws += " else if(obj.mode === 'create_tbutton'){ $('#row').append(\"<div class='two columns card'><h5>\"+obj.tb_title+\"</h5><hr/><button id=\"+\"tb\"+obj.index+\"></button></div>\");}";
  ws += " else if(obj.mode === 't_button_startup'){ var tb_index = obj.index; for(i=0; i<tb_index; i++){ var tb_index2 = \"tb\"+i; var tb_status = obj[tb_index2]; var tb_index3 = \"#\"+tb_index2;  console.log(tb_status); if(tb_status === \"1\"){$(tb_index3).toggleClass('button-primary'); $(tb_index3).text('Turn Off');} else if(tb_status === \"0\"){$(tb_index3).toggleClass('button'); $(tb_index3).text('Turn On');} } }";
  ws += " else if(obj.mode === 't_button_click'){ var tb_index = \"tb\"+obj.index; var tb_status = obj[tb_index]; var tb_index3 = \"#\"+tb_index;  console.log(tb_status); if(tb_status == \"1\"){ console.log(\"Status Tuned ON\");  $(tb_index3).text('Turn Off'); $(tb_index3).removeClass( \"button\" ).addClass( \"button-primary\" );} else if(tb_status == \"0\"){console.log(\"Status Tuned OFF\"); $(tb_index3).text('Turn On'); $(tb_index3).removeClass( \"button-primary\" ).addClass( \"button\" ); } } };";
  server->send(200, "application/javascript", ws);
}

// Handle Websockets Communication ( Out of Class)
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
              Serial.printf("[WSc] Connected to url: %s\n",  payload);
				      Serial.println("Connected");
              EasyUI.handleWebpage();
              Serial.println("JSON Data Sent to Client!");
            }
            break;
        case WStype_TEXT:
            StaticJsonBuffer<200> jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(payload);
             String mode = root["mode"];
             if(mode == "check_tb_status"){
               EasyUI.tbuttonStatus();
             }
             else if(mode == "tb_click"){
               String status = root["status"];
               String index = root["index"];
              EasyUI.tbClick(index, status);
             }
			           // send message to server
			          // webSocket.sendTXT("message here");
            break;
    }
}

void EasyUIClass::detectCDN(bool _autoState){
  CDN = _autoState;
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

// Create Toggle Buttons
void EasyUIClass::toggleButton(uint8_t  pin, const char* tbutton_label, int start_state, bool swap_state){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, start_state);
  tbutton_swap[tb_index] = swap_state;
  tbutton_pinout[tb_index] = pin;
  tbuttontitle[tb_index] = tbutton_label;
  tb_index++;
}

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

// Handle Index HTML
void EasyUIClass::handleRoot(){
  bool internet = false;
if(CDN){
  HTTPClient http;
  http.begin("http://www.google.com/");
  int httpCode = http.GET();
  if(httpCode > 0) {
    internet = true;
  }
}
  server->setContentLength(CONTENT_LENGTH_UNKNOWN);
  server->send(200,"text/html", "");
  server->sendContent(HTML_HEAD1);
  server->sendContent(String("<title>")+ui_title+"</title>");
  server->sendContent(HTML_HEAD2);
  server->sendContent(String("<h4>")+ui_title+"  <span id=\"connection_status\" class=\"label\">Offline</span></h4></div><hr />");
  server->sendContent(HTML_BODY);

    if(internet){
      server->sendContent("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/2.2.4/jquery.min.js\"></script>");
    }
    else{
      server->sendContent("<script src=\"/js/jquery.js\"></script>");
    }

  server->sendContent(HTML_END);
  server->client().stop();
}

// Handle Main Style CSS
void EasyUIClass::handleSCSS(){
server->send_P(200, "text/css", CSS_STYLE);
}

// Handle Normalize CSS
void EasyUIClass::handleNCSS(){
server->send_P(200, "text/css", CSS_NORMALIZE);
}

// Handle Jquery
void EasyUIClass::handleJS(){
  server->send_P(200, "application/javascript", JS_JQUERY);
}

// Handle Not found Page
void EasyUIClass::handleNotFound() {
  String url = "";
  String type = "";
  url = server->uri();
  if (SPIFFS.exists(url)) {
    File file = SPIFFS.open(url, "r");

    if (url.endsWith(".htm")) type = "text/html";
    else if (url.endsWith(".html")) type = "text/html";
    else if (url.endsWith(".css")) type = "text/css";
    else if (url.endsWith(".js")) type = "application/javascript";
    else if (url.endsWith(".png")) type = "image/png";
    else if (url.endsWith(".gif")) type = "image/gif";
    else if (url.endsWith(".jpg")) type = "image/jpeg";
    else if (url.endsWith(".ico")) type = "image/x-icon";
    else if (url.endsWith(".xml")) type = "text/xml";
    else if (url.endsWith(".pdf")) type = "application/x-pdf";
    else if (url.endsWith(".zip")) type = "application/x-zip";
    else if (url.endsWith(".gz")) type = "application/x-gzip";
    else type =  "text/plain";

    server->streamFile(file, type);
    file.close();
  }
  else {
    server->send(200, "text/html", "<html><h1>404 - You are Lost</h1></html>");
  }
}

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

void EasyUIClass::begin(){
    server.reset(new ESP8266WebServer(80));
    webSocket.reset(new WebSocketsServer(81,"","easyui"));
    SPIFFS.begin();
    server->on("/", std::bind(&EasyUIClass::handleRoot, this));
    server->on("/css/normalize.css", std::bind(&EasyUIClass::handleNCSS, this));
    server->on("/css/skeleton.css", std::bind(&EasyUIClass::handleSCSS, this));
    server->on("/js/jquery.js", std::bind(&EasyUIClass::handleJS, this));
    server->on("/js/sockets.js", std::bind(&EasyUIClass::handleSockets, this));
    server->onNotFound(std::bind(&EasyUIClass::handleNotFound, this));
    server->begin();
    webSocket->begin();
    webSocket->onEvent(webSocketEvent);
}



void EasyUIClass::loop(){
  webSocket->loop();
  server->handleClient();
}


EasyUIClass EasyUI;
