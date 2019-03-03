#ifndef ESPUI_h
#define ESPUI_h

#define DEBUG_ESPUI true
#define WS_AUTHENTICATION false

#include "Arduino.h"
#include "ArduinoJson.h"
#include "FS.h"
#include "stdlib_noniso.h"

#if defined(ESP32)

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "WiFi.h"

#else

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Hash.h>
#include <SPIFFSEditor.h>

#define FILE_WRITE "w"

#endif

// Message Types (and control types)

enum ControlType : uint8_t {
  Title = 0,
  Button,
  Pad,
  Cpad,
  Label,
  Switcher,
  Slider,
  Number,
  Text,
  Graph,
  GraphPoint,

  UpdateOffset = 100,
  UpdateLabel = 104,
  UpdateSwitcher,
  UpdateSlider,
  UpdateNumber,
  UpdateText,
  ClearGraph,

  InitialGui = 200
};

#define UI_INITIAL_GUI      ControlType::InitialGui

#define UI_TITLE            ControlType::Title
#define UI_LABEL            ControlType::Label
#define UI_BUTTON           ControlType::Button
#define UI_SWITCHER         ControlType::Switcher
#define UI_PAD              ControlType::Pad
#define UI_CPAD             ControlType::Cpad
#define UI_SLIDER           ControlType::Slider
#define UI_NUMBER           ControlType::Number
#define UI_TEXT_INPUT       ControlType::Text
#define UI_GRAPH            ControlType::Graph
#define UI_ADD_GRAPH_POINT  ControlType::GraphPoint

#define UPDATE_LABEL        ControlType::UpdateLabel
#define UPDATE_SWITCHER     ControlType::UpdateSwitcher
#define UPDATE_SLIDER       ControlType::UpdateSlider
#define UPDATE_NUMBER       ControlType::UpdateNumber
#define UPDATE_TEXT_INPUT   ControlType::UpdateText
#define CLEAR_GRAPH         ControlType::ClearGraph

// Colors
enum ControlColor : uint8_t {
  Turquoise,
  Emerald,
  Peterriver,
  Wetasphalt,
  Sunflower,
  Carrot,
  Alizarin,
  Dark,
  None = 0xFF
};
#define COLOR_TURQUOISE     ControlColor::Turquoise
#define COLOR_EMERALD       ControlColor::Emerald
#define COLOR_PETERRIVER    ControlColor::Peterriver
#define COLOR_WETASPHALT    ControlColor::Wetasphalt
#define COLOR_SUNFLOWER     ControlColor::Sunflower
#define COLOR_CARROT        ControlColor::Carrot
#define COLOR_ALIZARIN      ControlColor::Alizarin
#define COLOR_DARK          ControlColor::Dark
#define COLOR_NONE          ControlColor::None

class Control {
  public:
    ControlType type;
    unsigned int id;  // just mirroring the id here for practical reasons
    const char* label;
    void ( *callback )( Control, int );
    String value;
    ControlColor color;
    Control* next;

    Control(
      ControlType type, const char* label,
      void ( *callback )( Control, int ),
      String value, ControlColor color )
      : type( type ), label( label ), callback( callback ), value( value ), color( color ), next( nullptr ) {
      id = idCounter++;
    }

    Control( const Control& control )
      : type( control.type ), id( control.id ), label( control.label ),
        callback( control.callback ), value( control.value ),
        color( control.color ), next( control.next ) {}

  private:
    static uint16_t idCounter;
};


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
#define N_VALUE 9
#define T_VALUE 10

enum Verbosity : uint8_t {
  Quiet = 0,
  Verbose,
  VerboseJSON
};

class ESPUIClass {
  public:
    ESPUIClass( Verbosity verbosity = Verbosity::Quiet )
      : verbosity( verbosity ) {}

//     void begin( const char* _title, bool enableDebug=false ); // Setup servers and page in Memorymode
    void begin( const char* _title, const char* username = nullptr, const char* password = nullptr );

//     void beginSPIFFS( const char* _title, bool enableDebug=false ); // Setup servers and page in SPIFFSmode
    void beginSPIFFS( const char* _title, const char* username = nullptr, const char* password = nullptr );

    void prepareFileSystem();  // Initially preps the filesystem and loads a lot
    // of stuff into SPIFFS
    void list();
    // Creating Elements

    int addControl( ControlType type, const char* label,
                    String value = String( "" ), ControlColor color = ControlColor::Turquoise,
                    void ( *callback )( Control, int ) = nullptr );

    int button( const char* label,
                void ( *callback )( Control, int ), ControlColor color,
                String value = "" ); // Create Event Button
    int switcher( const char* label, bool startState,
                  void ( *callback )( Control, int ),
                  ControlColor color ); // Create Toggle Button
    int pad( const char* label, bool centerButton,
             void ( *callback )( Control, int ),
             ControlColor color ); // Create Pad Control
    int slider( const char* label,
                void ( *callback )( Control, int ),
                ControlColor color, String value ); // Create Slider Control
    int number( const char* label,
                void ( *callback )( Control, int ),
                ControlColor color, int number, int min, int max ); // Create a Number Input Control
    int text( const char* label,
              void ( *callback )( Control, int ),
              ControlColor color, String value = "" ); // Create a Text Input Control

    // Output only
    int label( const char* label, ControlColor color, String value = "" ); // Create Label
    int graph( const char* label, ControlColor color ); // Create Graph display

    // Update Elements

    Control* getControl( int id );
    Control* getControl( String label );

    // Update Elements
    void updateControl( int id, String value, int clientId = -1 );
    void updateControl( String label, String value, int clientId = -1 );
    void updateControl( Control* control, String value, int clientId = -1 );

    void print( int id, String value );
    void print( String label, String value );

    void updateLabel( int id, String value );
    void updateLabel( String label, String value );

    void updateSwitcher( int id, bool nValue, int clientId = -1 );
    void updateSwitcher( String label, bool nValue, int clientId = -1 );

    void updateSlider( int id, int nValue, int clientId = -1 );
    void updateSlider( String label, int nValue, int clientId = -1 );

    void updateNumber( int id, int nValue, int clientId = -1 );
    void updateNumber( String label, int nValue, int clientId = -1 );

    void updateText( int id, String nValue, int clientId = -1 );
    void updateText( String label, String nValue, int clientId = -1 );

    void clearGraph( int id, int clientId = -1 );
    void clearGraph( String label, int clientId = -1 );

    void addGraphPoint( int id, int nValue, int clientId = -1 );
    void addGraphPoint( String label, int nValue, int clientId = -1 );

//     void textThem( String text, int clientId = -1 );

    // Variables ---
    const char* ui_title = "ESPUI";  // Store UI Title and Header Name
    int cIndex = 0;                  // Control index
    Control* controls = nullptr;
    void jsonDom( AsyncWebSocketClient* client );

    Verbosity verbosity;

  private:
    const char* basicAuthUsername = nullptr;
    const char* basicAuthPassword = nullptr;
    bool basicAuth = true;
    AsyncWebServer* server;
    AsyncWebSocket* ws;
};

extern ESPUIClass ESPUI;
#endif
