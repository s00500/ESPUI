#include "ESPUI.h"

#include "dataIndexHTML.h"

#include "dataNormalizeCSS.h"
#include "dataStyleCSS.h"

#include "dataControlsJS.h"
#include "dataSliderJS.h"
#include "dataZeptoJS.h"

#include <ESPAsyncWebServer.h>
#include <functional>

uint16_t Control::idCounter = 0;

// ################# Spiffs functions
#if defined(ESP32)
void listDir( const char* dirname, uint8_t levels ) {
  if ( ESPUI.verbosity ) {
    Serial.printf( "Listing directory: %s\n", dirname );
  }

  File root = SPIFFS.open( dirname );

  if ( !root ) {
    if ( ESPUI.verbosity ) {
      Serial.println( "Failed to open directory" );
    }

    return;
  }

  if ( !root.isDirectory() ) {
    if ( ESPUI.verbosity ) {
      Serial.println( "Not a directory" );
    }

    return;
  }

  File file = root.openNextFile();

  while ( file ) {
    if ( file.isDirectory() ) {
      if ( ESPUI.verbosity ) {
        Serial.print( "  DIR : " );
        Serial.println( file.name() );
      }

      if ( levels ) {
        listDir( file.name(), levels - 1 );
      }
    } else {
      if ( ESPUI.verbosity ) {
        Serial.print( "  FILE: " );
        Serial.print( file.name() );
        Serial.print( "  SIZE: " );
        Serial.println( file.size() );
      }
    }

    file = root.openNextFile();
  }
}
#else

void listDir( const char* dirname, uint8_t levels ) {
  // ignoring levels for esp8266
  Serial.printf( "Listing directory: %s\n", dirname );

  String str = "";
  Dir dir = SPIFFS.openDir( "/" );

  while ( dir.next() ) {
    Serial.print( "  FILE: " );
    Serial.print( dir.fileName() );
    Serial.print( "  SIZE: " );
    Serial.println( dir.fileSize() );
  }
}

#endif

void ESPUIClass::list() {
  if ( !SPIFFS.begin() ) {
    Serial.println( "SPIFFS Mount Failed" );
    return;
  }

  listDir( "/", 1 );
#if defined(ESP32)

  Serial.println( SPIFFS.totalBytes() );
  Serial.println( SPIFFS.usedBytes() );

#else
  FSInfo fs_info;
  SPIFFS.info( fs_info );

  Serial.println( fs_info.totalBytes );
  Serial.println( fs_info.usedBytes );

#endif
}

void deleteFile( const char* path ) {
  if ( ESPUI.verbosity ) {
    Serial.print( SPIFFS.exists( path ) );
  }

  if ( !SPIFFS.exists( path ) ) {
    if ( ESPUI.verbosity ) {
      Serial.printf( "File: %s does not exist, not deleting\n", path );
    }

    return;
  }

  if ( ESPUI.verbosity ) {
    Serial.printf( "Deleting file: %s\n", path );
  }

  if ( SPIFFS.remove( path ) ) {
    if ( ESPUI.verbosity ) {
      Serial.println( "File deleted" );
    }
  } else {
    if ( ESPUI.verbosity ) {
      Serial.println( "Delete failed" );
    }
  }
}

void writeFile( const char* path, const char* data ) {
  if ( ESPUI.verbosity ) {
    Serial.printf( "Writing file: %s\n", path );
  }

  File file = SPIFFS.open( path, FILE_WRITE );

  if ( !file ) {
    if ( ESPUI.verbosity ) {
      Serial.println( "Failed to open file for writing" );
    }

    return;
  }

#if defined(ESP32)

  if ( file.print( data ) ) {
    if ( ESPUI.verbosity ) {
      Serial.println( "File written" );
    }
  } else {
    if ( ESPUI.verbosity ) {
      Serial.println( "Write failed" );
    }
  }

#else

  if ( file.print( FPSTR( data ) ) ) {
    if ( ESPUI.verbosity ) {
      Serial.println( "File written" );
    }
  } else {
    if ( ESPUI.verbosity ) {
      Serial.println( "Write failed" );
    }
  }

#endif
  file.close();
}

// end Spiffs functions

void ESPUIClass::prepareFileSystem() {
  // this function should only be used once

  if ( this->verbosity ) {
    Serial.println( "About to prepare filesystem..." );
  }

#if defined(ESP32)
  SPIFFS.format();

  if ( !SPIFFS.begin( true ) ) {
    if ( this->verbosity ) {
      Serial.println( "SPIFFS Mount Failed" );
    }

    return;
  }

  if ( this->verbosity ) {
    listDir( "/", 1 );
    Serial.println( "SPIFFS Mount ESP32 Done" );
  }

#else
  SPIFFS.format();
  SPIFFS.begin();

  if ( this->verbosity ) {
    Serial.println( "SPIFFS Mount ESP8266 Done" );
  }

#endif

  deleteFile( "/index.htm" );

  deleteFile( "/css/style.css" );
  deleteFile( "/css/normalize.css" );

  deleteFile( "/js/zepto.min.js" );
  deleteFile( "/js/controls.js" );
  deleteFile( "/js/slider.js" );

  if ( this->verbosity ) {
    Serial.println( "Cleanup done" );
  }

  // Now write
  writeFile( "/index.htm", HTML_INDEX );

  writeFile( "/css/style.css", CSS_STYLE );
  writeFile( "/css/normalize.css", CSS_NORMALIZE );

  writeFile( "/js/zepto.min.js", JS_ZEPTO );
  writeFile( "/js/controls.js", JS_CONTROLS );
  writeFile( "/js/slider.js", JS_SLIDER );

  if ( this->verbosity ) {
    Serial.println( "Done Initializing filesystem :-)" );
  }

#if defined(ESP32)

  if ( this->verbosity ) {
    listDir( "/", 1 );
  }

#endif

  SPIFFS.end();
}

// Handle Websockets Communication
void onWsEvent( AsyncWebSocket* server, AsyncWebSocketClient* client,
                AwsEventType type, void* arg, uint8_t* data, size_t len ) {
  switch ( type ) {
    case WS_EVT_DISCONNECT: {
      if ( ESPUI.verbosity ) {
        Serial.printf( "Disconnected!\n" );
      }

      break;
    }

    case WS_EVT_PONG: {
      if ( ESPUI.verbosity ) {
        Serial.printf( "Received PONG!\n" );
      }

      break;
    }

    case WS_EVT_ERROR: {
      if ( ESPUI.verbosity ) {
        Serial.printf( "WebSocket Error!\n" );
      }

      break;
    }

    case WS_EVT_CONNECT: {
      if ( ESPUI.verbosity ) {
        Serial.print( "Connected: " );
        Serial.println( client->id() );
      }

      ESPUI.jsonDom( client );

      if ( ESPUI.verbosity ) {
        Serial.println( "JSON Data Sent to Client!" );
      }
    }
    break;

    case WS_EVT_DATA: {
      String msg = "";

      for ( size_t i = 0; i < len; i++ ) {
        msg += ( char )data[i];
      }

      Control* c = ESPUI.getControl( msg.substring( msg.lastIndexOf( ':' ) + 1 ).toInt() );

      if ( c != nullptr && c->callback != nullptr ) {
        if ( msg.startsWith( "bdown:" ) ) {
          c->callback( *c, B_DOWN );
        } else if ( msg.startsWith( "bup:" ) ) {
          c->callback( *c, B_UP );
        } else if ( msg.startsWith( "pfdown:" ) ) {
          c->callback( *c, P_FOR_DOWN );
        } else if ( msg.startsWith( "pfup:" ) ) {
          c->callback( *c, P_FOR_UP );
        } else if ( msg.startsWith( "pldown:" ) ) {
          c->callback( *c, P_LEFT_DOWN );
        } else if ( msg.startsWith( "plup:" ) ) {
          c->callback( *c, P_LEFT_UP );
        } else if ( msg.startsWith( "prdown:" ) ) {
          c->callback( *c, P_RIGHT_DOWN );
        } else if ( msg.startsWith( "prup:" ) ) {
          c->callback( *c, P_RIGHT_UP );
        } else if ( msg.startsWith( "pbdown:" ) ) {
          c->callback( *c, P_BACK_DOWN );
        } else if ( msg.startsWith( "pbup:" ) ) {
          c->callback( *c, P_BACK_UP );
        } else if ( msg.startsWith( "pcdown:" ) ) {
          c->callback( *c, P_CENTER_DOWN );
        } else if ( msg.startsWith( "pcup:" ) ) {
          c->callback( *c, P_CENTER_UP );
        } else if ( msg.startsWith( "sactive:" ) ) {
          ESPUI.updateSwitcher( c->id, true );
          c->callback( *c, S_ACTIVE );
        } else if ( msg.startsWith( "sinactive:" ) ) {
          ESPUI.updateSwitcher( c->id, false );
          c->callback( *c, S_INACTIVE );
        } else if ( msg.startsWith( "slvalue:" ) ) {
          int value = msg.substring( msg.indexOf( ':' ) + 1, msg.lastIndexOf( ':' ) ).toInt();
          ESPUI.updateSlider( c->id, value, client->id() );
          c->callback( *c, SL_VALUE );
        } else if ( msg.startsWith( "nvalue:" ) ) {
          int value = msg.substring( msg.indexOf( ':' ) + 1, msg.lastIndexOf( ':' ) ).toInt();
          ESPUI.updateNumber( c->id, value, client->id() );
          c->callback( *c, N_VALUE );
        } else if ( msg.startsWith( "tvalue:" ) ) {
          String value = msg.substring( msg.indexOf( ':' ) + 1, msg.lastIndexOf( ':' ) );
          ESPUI.updateText( c->id, value, client->id() );
          c->callback( *c, T_VALUE );
        }
      } else {
        if ( ESPUI.verbosity ) {
          Serial.println( "Maleformated id in websocket message" );
        }

        return;
      }
    }
    break;

    default:
      break;
  }
}

int ESPUIClass::addControl( ControlType type, const char* label,
                            void ( *callback )( Control, int ),
                            String value, ControlColor color ) {
  if ( this->getControl( label ) != nullptr ) {
    if ( this->verbosity ){
      Serial.println( "UI ERROR: Element " + String( label ) +
                      " exists, skipping creating element!" );}

    return -1;
  }

  Control* control = new Control( type, label, callback, value, color );

  if ( this->controls == nullptr ) {
    this->controls = control;
  } else {
    Control* iterator = this->controls;

    while ( iterator->next != nullptr ) {
      iterator = iterator->next;
    }

    iterator->next = control;
  }

  return control->id;
}

int ESPUIClass::label( const char* label, ControlColor color, String value ) {
  return addControl( ControlType::Label, label, nullptr, value, color );
}

int ESPUIClass::graph( const char* label, ControlColor color ) {
  return addControl( ControlType::Graph, label, nullptr, "", color );
}

// TODO: this still needs a range setting
int ESPUIClass::slider( const char* label, void ( *callback )( Control, int ),
                        ControlColor color, String value ) {
  return addControl( ControlType::Button, label, callback, "", color );
}

int ESPUIClass::button( const char* label, void ( *callback )( Control, int ),
                        ControlColor color, String value ) {
  return addControl( ControlType::Button, label, callback, value, color );
}

int ESPUIClass::switcher( const char* label, bool startState,
                          void ( *callback )( Control, int ), ControlColor color ) {
  return addControl( ControlType::Switcher, label, callback, "", color );
}

int ESPUIClass::pad( const char* label, bool center,
                     void ( *callback )( Control, int ), ControlColor color ) {
  if ( center ) {
    return addControl( ControlType::Cpad, label, callback, "", color );
  } else {
    return addControl( ControlType::Pad, label, callback, "", color );
  }
}

// TODO: min and max need to be saved, they also need to be sent to the frontend
int ESPUIClass::number( const char* label, void ( *callback )( Control, int ),
                        ControlColor color, int number, int min, int max ) {
  return addControl( ControlType::Number, label, callback, String( number ), color );
}

int ESPUIClass::text( const char* label, void ( *callback )( Control, int ),
                      ControlColor color, String value ) {
  return addControl( ControlType::Text, label, callback, value, color );
}


Control* ESPUIClass::getControl( int id ) {
  Control* control = this->controls;

  while ( control != nullptr ) {
    if ( control->id == id ) {
      return control;
    }

    control = control->next;
  }

  return nullptr;
}

Control* ESPUIClass::getControl( String label ) {
  Control* control = this->controls;

  while ( control != nullptr ) {
    if ( String( control->label ) == label ) {
      return control;
    }

    control = control->next;
  }

  return nullptr;
}

void ESPUIClass::updateControl( Control* control, String value, int clientId ) {
  if ( control ) {
    control->value = value;
    String json;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["type"] = ( int )control->type + ControlType::UpdateOffset;
    root["value"] = control->value;
    root["id"] = control->id;
    root.printTo( json );

    if ( clientId > 0 ) {
      // This is a hacky workaround because ESPAsyncWebServer does not have a function
      // like this and it's clients array is private
      int tryId = 0;

      for ( int count = 0; count < this->ws->count(); ) {
        if ( this->ws->hasClient( tryId ) ) {
          if ( clientId != tryId ) {
            this->ws->client( tryId )->text( json );

            if ( this->verbosity >= Verbosity::VerboseJSON ) {
              Serial.println( json );
            }
          }

          count++;
        }

        tryId++;
      }
    } else {
      if ( this->verbosity >= Verbosity::VerboseJSON ) {
        Serial.println( json );
      }

      this->ws->textAll( json );
    }
  }
}
void ESPUIClass::updateControl( int id, String value, int clientId ) {
  Control* control = getControl( id );

  if ( control ) {
    updateControl( control, value, clientId );
  } else {
    if ( this->verbosity ) {
      Serial.println( String( "Error: There is no control with ID " ) + String( id ) );
    }
  }
}
void ESPUIClass::updateControl( String label, String value, int clientId ) {
  Control* control = getControl( label );

  if ( control ) {
    updateControl( control, value, clientId );
  } else {
    if ( this->verbosity ) {
      Serial.println( String( "Error: There is no control with label " ) + label );
    }
  }
}



void ESPUIClass::print( int id, String value ) {
  updateControl( id, value );
}

void ESPUIClass::print( String label, String value ) {
  updateControl( label, value );
}

void ESPUIClass::updateLabel( int id, String value ) {
  updateControl( id, value );
}

void ESPUIClass::updateLabel( String label, String value ) {
  updateControl( label, value );
}

void ESPUIClass::updateSlider( int id, int nValue, int clientId ) {
  updateControl( id, String( nValue ), clientId );
}

void ESPUIClass::updateSlider( String label, int nValue, int clientId ) {
  updateControl( label, String( nValue ), clientId );
}

void ESPUIClass::updateSwitcher( int id, bool nValue, int clientId ) {
  updateControl( id, String( int( nValue ? 1 : 0 ) ), clientId );
}

void ESPUIClass::updateSwitcher( String label, bool nValue, int clientId ) {
  updateControl( label, String( int( nValue ? 1 : 0 ) ), clientId );
}

void ESPUIClass::updateNumber( int id, int number, int clientId ) {
  updateControl( id, String( number ), clientId );
}

void ESPUIClass::updateNumber( String label, int number, int clientId ) {
  updateControl( label, String( number ), clientId );
}

void ESPUIClass::updateText( int id, String text, int clientId ) {
  updateControl( id, text, clientId );
}

void ESPUIClass::updateText( String label, String text, int clientId ) {
  updateControl( label, text, clientId );
}

/*
Convert & Transfer Arduino elements to JSON elements
Initially this function used to send the control element data individually.
Due to a change in the ESPAsyncWebserver library this had top be changed to be
sent as one blob at the beginning. Therefore a new type is used as well
*/
void ESPUIClass::jsonDom( AsyncWebSocketClient* client ) {
  String json;
  DynamicJsonBuffer jsonBuffer( 2000 );
  JsonObject& root = jsonBuffer.createObject();
  root["type"] = ( int )UI_INITIAL_GUI;
  JsonArray& items = jsonBuffer.createArray();

  Control* control = this->controls;

  {
    JsonObject& item = jsonBuffer.createObject();
    item["type"] = ( int )UI_TITLE;
    item["label"] = String( ui_title );
    items.add( item );
  }

  while ( control != nullptr ) {
    JsonObject& item = jsonBuffer.createObject();

    item["id"] = String( control->id );
    item["type"] = ( int )control->type;
    item["label"] = String( control->label );
    item["value"] = String( control->value );
    item["color"] = ( int )control->color;

    items.add( item );

    control = control->next;
  }

  // Send as one big bunch
  root["controls"] = items;
  root.printTo( json );

  if ( this->verbosity >= Verbosity::VerboseJSON ) {
    Serial.println( json );
  }

  client->text( json );
}

void ESPUIClass::beginSPIFFS( const char* _title, const char* username, const char* password ) {
  ui_title = _title;
  this->basicAuthUsername = username;
  this->basicAuthPassword = password;

  if ( username == nullptr && password == nullptr ) {
    basicAuth = false;
  } else {
    basicAuth = true;
  }

  server = new AsyncWebServer( 80 );
  ws = new AsyncWebSocket( "/ws" );

  if ( !SPIFFS.begin() ) {
    if ( ESPUI.verbosity ) {
      Serial.println(
        "SPIFFS Mount Failed, PLEASE CHECK THE README ON HOW TO "
        "PREPARE YOUR ESP!!!!!!!" );
    }

    return;
  }

  if ( ESPUI.verbosity ) {
    listDir( "/", 1 );
  }

  if ( !SPIFFS.exists( "/index.htm" ) ) {
    if ( ESPUI.verbosity ) {
      Serial.println(
        "Please read the README!!!!!!!, Make sure to "
        "ESPUI.prepareFileSystem() once in an empty sketch" );
    }

    return;
  }

  ws->onEvent( onWsEvent );
  server->addHandler( ws );

  if ( basicAuth ) {

    if ( WS_AUTHENTICATION ) {
      ws->setAuthentication( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword );
    }

    server->serveStatic( "/", SPIFFS, "/" )
    .setDefaultFile( "index.htm" )
    .setAuthentication( username, password );

  } else {
    server->serveStatic( "/", SPIFFS, "/" )
    .setDefaultFile( "index.htm" );
  }

  // Heap for general Servertest
  server->on( "/heap", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    request->send( 200, "text/plain",
                   String( ESP.getFreeHeap() ) + " In SPIFFSmode" );
  } );

  server->onNotFound(
  []( AsyncWebServerRequest * request ) {
    request->send( 404 );
  } );

  server->begin();

  if ( this->verbosity ) {
    Serial.println( "UI Initialized" );
  }
}

void ESPUIClass::begin( const char* _title, const char* username, const char* password ) {
  basicAuthUsername = username;
  basicAuthPassword = password;

  if ( username != nullptr && password != nullptr ) {
    basicAuth = true;
  } else {
    basicAuth = false;
  }

  ui_title = _title;

  server = new AsyncWebServer( 80 );
  ws = new AsyncWebSocket( "/ws" );

  ws->onEvent( onWsEvent );
  server->addHandler( ws );

  if ( basicAuth && WS_AUTHENTICATION )
    ws->setAuthentication( username, password );


  server->on( "/", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    AsyncWebServerResponse* response =
      request->beginResponse_P( 200, "text/html", HTML_INDEX );
    request->send( response );
  } );

  // Javascript files

  server->on( "/js/zepto.min.js", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    AsyncWebServerResponse* response =
      request->beginResponse_P(
        200, "application/javascript", JS_ZEPTO_GZIP, sizeof( JS_ZEPTO_GZIP ) );
    response->addHeader( "Content-Encoding", "gzip" );
    request->send( response );
  } );

  server->on( "/js/controls.js", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    AsyncWebServerResponse* response =
      request->beginResponse_P(
        200, "application/javascript", JS_CONTROLS_GZIP, sizeof( JS_CONTROLS_GZIP ) );
    response->addHeader( "Content-Encoding", "gzip" );
    request->send( response );
  } );

  server->on( "/js/slider.js", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    AsyncWebServerResponse* response =
      request->beginResponse_P(
        200, "application/javascript", JS_SLIDER_GZIP, sizeof( JS_SLIDER_GZIP ) );
    response->addHeader( "Content-Encoding", "gzip" );
    request->send( response );
  } );

  // Stylesheets

  server->on( "/css/style.css", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    AsyncWebServerResponse* response =
      request->beginResponse_P(
        200, "text/css", CSS_STYLE_GZIP, sizeof( CSS_STYLE_GZIP ) );
    response->addHeader( "Content-Encoding", "gzip" );
    request->send( response );
  } );

  server->on(
  "/css/normalize.css", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    AsyncWebServerResponse* response =
      request->beginResponse_P(
        200, "text/css", CSS_NORMALIZE_GZIP, sizeof( CSS_NORMALIZE_GZIP ) );
    response->addHeader( "Content-Encoding", "gzip" );
    request->send( response );
  } );

  // Heap for general Servertest
  server->on( "/heap", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    request->send( 200, "text/plain",
                   String( ESP.getFreeHeap() ) + " In Memorymode" );
  } );

  server->onNotFound(
  []( AsyncWebServerRequest * request ) {
    request->send( 404 );
  } );

  server->begin();

  if ( this->verbosity ) {
    Serial.println( "UI Initialized" );
  }
}
