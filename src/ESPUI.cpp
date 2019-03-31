#include "ESPUI.h"

#include "dataIndexHTML.h"

#include "dataNormalizeCSS.h"
#include "dataStyleCSS.h"

#include "dataControlsJS.h"
#include "dataSliderJS.h"
#include "dataTabbedcontentJS.h"
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
  deleteFile( "/js/tabbedcontent.js" );

  if ( this->verbosity ) {
    Serial.println( "Cleanup done" );
  }

  // Now write
  writeFile( "/index.htm", HTML_INDEX );

  writeFile( "/css/style.css", CSS_STYLE );
  writeFile( "/css/normalize.css", CSS_NORMALIZE );

  writeFile( "/js/zepto.min.js", JS_ZEPTO );
  writeFile( "/js/controls.js", JS_CONTROLS );
  writeFile( "/js/slider.js", JS_SLIDER );;
  writeFile( "/js/tabbedcontent.js", JS_TABBEDCONTENT );

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
      msg.reserve( len + 1 );

      for ( size_t i = 0; i < len; i++ ) {
        msg += ( char )data[i];
      }

      uint16_t id =  msg.substring( msg.lastIndexOf( ':' ) + 1 ).toInt();

      if ( ESPUI.verbosity >= Verbosity::VerboseJSON ) {
        Serial.print( "WS rec: " );
        Serial.println( msg );
        Serial.print( "WS recognised ID: " );
        Serial.println( id );
      }

      Control* c = ESPUI.getControl( id );

      if ( c == nullptr ) {
        if ( ESPUI.verbosity ) {
          Serial.print( "No control found for ID " );
          Serial.println( id );
        }

        return;
      }

      if ( c->callback == nullptr ) {
        if ( ESPUI.verbosity ) {
          Serial.print( "No callback found for ID " );
          Serial.println( id );
        }

        return;
      }

      if ( msg.startsWith( "bdown:" ) ) {
        c->callback( c, B_DOWN );
      } else if ( msg.startsWith( "bup:" ) ) {
        c->callback( c, B_UP );
      } else if ( msg.startsWith( "pfdown:" ) ) {
        c->callback( c, P_FOR_DOWN );
      } else if ( msg.startsWith( "pfup:" ) ) {
        c->callback( c, P_FOR_UP );
      } else if ( msg.startsWith( "pldown:" ) ) {
        c->callback( c, P_LEFT_DOWN );
      } else if ( msg.startsWith( "plup:" ) ) {
        c->callback( c, P_LEFT_UP );
      } else if ( msg.startsWith( "prdown:" ) ) {
        c->callback( c, P_RIGHT_DOWN );
      } else if ( msg.startsWith( "prup:" ) ) {
        c->callback( c, P_RIGHT_UP );
      } else if ( msg.startsWith( "pbdown:" ) ) {
        c->callback( c, P_BACK_DOWN );
      } else if ( msg.startsWith( "pbup:" ) ) {
        c->callback( c, P_BACK_UP );
      } else if ( msg.startsWith( "pcdown:" ) ) {
        c->callback( c, P_CENTER_DOWN );
      } else if ( msg.startsWith( "pcup:" ) ) {
        c->callback( c, P_CENTER_UP );
      } else if ( msg.startsWith( "sactive:" ) ) {
        ESPUI.updateSwitcher( c->id, true );
        c->callback( c, S_ACTIVE );
      } else if ( msg.startsWith( "sinactive:" ) ) {
        ESPUI.updateSwitcher( c->id, false );
        c->callback( c, S_INACTIVE );
      } else if ( msg.startsWith( "slvalue:" ) ) {
        c->value = msg.substring( msg.indexOf( ':' ) + 1, msg.lastIndexOf( ':' ) );
        ESPUI.updateControl( c );
        c->callback( c, SL_VALUE );
      } else if ( msg.startsWith( "nvalue:" ) ) {
        c->value = msg.substring( msg.indexOf( ':' ) + 1, msg.lastIndexOf( ':' ) );
        c->callback( c, N_VALUE );
      } else if ( msg.startsWith( "tvalue:" ) ) {
        c->value = msg.substring( msg.indexOf( ':' ) + 1, msg.lastIndexOf( ':' ) );
        c->callback( c, T_VALUE );
      } else if ( msg.startsWith( "svalue:" ) ) {
        c->value = msg.substring( msg.indexOf( ':' ) + 1, msg.lastIndexOf( ':' ) );
        c->callback( c, S_VALUE );
      } else {
        if ( ESPUI.verbosity ) {
          Serial.println( "Malformated message from the websocket" );
        }
      }
    }
    break;

    default:
      break;
  }
}

uint16_t ESPUIClass::addControl( ControlType type, const char* label,
                                 String value, ControlColor color,
                                 uint16_t parentControl,
                                 void ( *callback )( Control*, int )
                               ) {
  Control* control = new Control( type, label, callback, value, color, parentControl );

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
  return addControl( ControlType::Label, label, value, color );
}

int ESPUIClass::graph( const char* label, ControlColor color ) {
  return addControl( ControlType::Graph, label, "", color );
}

// TODO: this still needs a range setting
int ESPUIClass::slider( const char* label, void ( *callback )( Control*, int ),
                        ControlColor color, String value ) {
  return addControl( ControlType::Slider, label, "", color, Control::noParent, callback );
}

int ESPUIClass::button( const char* label, void ( *callback )( Control*, int ),
                        ControlColor color, String value ) {
  return addControl( ControlType::Button, label, value, color, Control::noParent, callback );
}

int ESPUIClass::switcher( const char* label, bool startState,
                          void ( *callback )( Control*, int ), ControlColor color ) {
  return addControl( ControlType::Switcher, label, "", color, Control::noParent, callback );
}

int ESPUIClass::pad( const char* label, bool center,
                     void ( *callback )( Control*, int ), ControlColor color ) {
  if ( center ) {
    return addControl( ControlType::PadWithCenter, label, "", color, Control::noParent, callback );
  } else {
    return addControl( ControlType::Pad, label, "", color, Control::noParent, callback );
  }
}

// TODO: min and max need to be saved, they also need to be sent to the frontend
int ESPUIClass::number( const char* label, void ( *callback )( Control*, int ),
                        ControlColor color, int number, int min, int max ) {
  return addControl( ControlType::Number, label, String( number ), color, Control::noParent, callback );
}

int ESPUIClass::text( const char* label, void ( *callback )( Control*, int ),
                      ControlColor color, String value ) {
  return addControl( ControlType::Text, label, value, color, Control::noParent, callback );
}


Control* ESPUIClass::getControl( uint16_t id ) {
  Control* control = this->controls;

  while ( control != nullptr ) {
    if ( control->id == id ) {
      return control;
    }

    control = control->next;
  }

  return nullptr;
}

void ESPUIClass::addGraphPoint( Control* control, int line, int nValue, int clientId ) {
  if ( control ) {
    Serial.println( "addGraphPoint" );
    DynamicJsonBuffer jsonBuffer( 256 );
    JsonObject& root = jsonBuffer.createObject();

    root["type"] = ( int )ControlType::GraphPoint;
    root["value"] = nValue;
    root["id"] = control->id;
    root["line"] = line;
    size_t len = root.measureLength();

    AsyncWebSocketMessageBuffer* buffer = this->ws->makeBuffer( len ); //  creates a buffer (len + 1) for you.

    if ( buffer ) {
      root.printTo( ( char* )buffer->get(), len + 1 );

      this->ws->textAll( buffer );
    }
  }
}

void ESPUIClass::addGraphPoint( uint16_t id, int line, int nValue, int clientId ) {
  Control* control = getControl( id );

  if ( control ) {
    addGraphPoint( control, line, nValue, clientId );
  } else {
    if ( this->verbosity ) {
      Serial.print( "Error: There is no control with ID " );
      Serial.println( String( id ) );
    }
  }
}

void ESPUIClass::updateControl( Control* control, int clientId ) {
  if ( control ) {
    constexpr size_t sizeOfBuffer = 200;

    StaticJsonBuffer<sizeOfBuffer> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["type"] = ( int )control->type + ControlType::UpdateOffset;
    root["value"] = control->value;
    root["id"] = control->id;
    root["color"] = ( int )control->color;
    size_t len = root.measureLength();

    char buffer[sizeOfBuffer];

    if ( len >= sizeof( buffer ) ) {
      len = sizeof( buffer ) - 1;
    }


//     AsyncWebSocketMessageBuffer* buffer = this->ws->makeBuffer( len ); //  creates a buffer (len + 1) for you.

    root.printTo( ( char* )buffer/*r->get()*/, len + 1 );

    if ( clientId > 0 ) {
      // This is a hacky workaround because ESPAsyncWebServer does not have a function
      // like this and it's clients array is private
      int tryId = 0;

      for ( int count = 0; count < this->ws->count(); ) {
        if ( this->ws->hasClient( tryId ) ) {
          if ( clientId != tryId ) {
            this->ws->client( tryId )->text( buffer );

            if ( this->verbosity >= Verbosity::VerboseJSON ) {
              Serial.println( buffer );
            }
          }

          count++;
        }

        tryId++;
      }
    } else {
        if ( this->verbosity >= Verbosity::VerboseJSON ) {
          Serial.println( buffer );
        }

      this->ws->textAll( buffer );
    }
  }
}

void ESPUIClass::updateControl( uint16_t id, int clientId ) {
  Control* control = getControl( id );

  if ( control ) {
    updateControl( control, clientId );
  } else {
    if ( this->verbosity ) {
      Serial.println( String( "Error: There is no control with ID " ) + String( id ) );
    }
  }
}

void ESPUIClass::updateControl( Control* control, String value, int clientId ) {
  if ( control ) {
    control->value = value;
    updateControl( control, clientId );
  }
}

void ESPUIClass::updateControl( uint16_t id, String value, int clientId ) {
  Control* control = getControl( id );

  if ( control ) {
    updateControl( control, value, clientId );
  } else {
    if ( this->verbosity ) {
      Serial.println( String( "Error: There is no control with ID " ) + String( id ) );
    }
  }
}

void ESPUIClass::print( uint16_t id, String value ) {
  updateControl( id, value );
}

void ESPUIClass::updateLabel( uint16_t id, String value ) {
  updateControl( id, value );
}

void ESPUIClass::updateSlider( uint16_t id, int nValue, int clientId ) {
  updateControl( id, String( nValue ), clientId );
}

void ESPUIClass::updateSwitcher( uint16_t id, bool nValue, int clientId ) {
  updateControl( id, String( nValue ? "1" : "0" ), clientId );
}

void ESPUIClass::updateNumber( uint16_t id, int number, int clientId ) {
  updateControl( id, String( number ), clientId );
}

void ESPUIClass::updateText( uint16_t id, String text, int clientId ) {
  updateControl( id, text, clientId );
}

void ESPUIClass::updateSelect( uint16_t id, String text, int clientId ) {
  updateControl( id, text, clientId );
}

/*
Convert & Transfer Arduino elements to JSON elements
Initially this function used to send the control element data individually.
Due to a change in the ESPAsyncWebserver library this had top be changed to be
sent as one blob at the beginning. Therefore a new type is used as well
*/
void ESPUIClass::jsonDom( AsyncWebSocketClient* client ) {
//   String json;
  DynamicJsonBuffer jsonBuffer( 4000 );
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

    if ( control->parentControl != Control::noParent ) {
      item["parentControl"] = String( control->parentControl );
    }

    // special case for selects: to preselect an option, you have to add "selected" to <option>
    if ( control->type == ControlType::Option ) {
      if ( ESPUI.getControl( control->parentControl )->value == control->value ) {
        item["selected"] = "selected";
      } else {
        item["selected"] = "";
      }
    }

    items.add( item );

    control = control->next;
  }

  // Send as one big bunch
  root["controls"] = items;

  size_t len = root.measureLength();

  AsyncWebSocketMessageBuffer* buffer = this->ws->makeBuffer( len ); //  creates a buffer (len + 1) for you.

  if ( buffer ) {
    root.printTo( ( char* )buffer->get(), len + 1 );
    client->text( buffer );
  }
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

  server->on( "/js/tabbedcontent.js", HTTP_GET, []( AsyncWebServerRequest * request ) {
    if ( ESPUI.basicAuth && !request->authenticate( ESPUI.basicAuthUsername, ESPUI.basicAuthPassword ) ) {
      return request->requestAuthentication();
    }

    AsyncWebServerResponse* response =
      request->beginResponse_P(
        200, "application/javascript", JS_TABBEDCONTENT_GZIP, sizeof( JS_TABBEDCONTENT_GZIP ) );
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
