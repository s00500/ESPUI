# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

ESPUI is an Arduino library that creates web-based user interfaces for ESP8266 and ESP32 microcontrollers. It uses WebSockets for real-time communication between the ESP device and web browsers, with a frontend built on Skeleton CSS and Zepto.js. The library works in standalone mode without requiring internet connectivity.

## Build and Development Commands

### PlatformIO (Recommended)
```bash
# Build for ESP32
pio run -e esp32

# Build for ESP8266
pio run -e esp8266

# Upload to device
pio run -e esp32 --target upload

# Monitor serial output
pio run -e esp32 --target monitor
```

The main PlatformIO configuration is at `pio_examples/gui/platformio.ini`.

### Arduino IDE
The library can be installed via the Arduino Library Manager (search for "ESPUI") or manually by placing the library in the Arduino libraries folder.

### Updating Frontend Assets
When modifying HTML, CSS, or JavaScript files in the `data/` directory:

```bash
# Go version (recommended, no external dependencies to install)
cd tools && go run prepare_static_ui_sources.go -a

# Or Python version (requires: pip install jsmin csscompressor htmlmin)
python3 tools/prepare_static_ui_sources.py -a
```

This script:
- Minifies HTML/CSS/JS files from `data/`
- Generates minified versions alongside originals
- Creates C header files in `src/` with PROGMEM constants containing gzipped data
- Headers like `dataIndexHTML.h`, `dataStyleCSS.h`, `dataControlsJS.h` are auto-generated

See `tools/README.md` for detailed usage and options.

**Important**: Always run this script after modifying frontend files. Include both the minified files and updated headers in commits.

## Architecture

### Core Class Structure

**ESPUIClass** (`src/ESPUI.h`, `src/ESPUI.cpp`)
- Main singleton class accessed via global `ESPUI` object
- Manages the AsyncWebServer instance and WebSocket connections
- Provides API for creating UI controls: `addControl()`, `button()`, `slider()`, `text()`, etc.
- Handles two modes: PROGMEM (default) and LittleFS for serving static assets
- Thread-safe on ESP32 using a semaphore for control list access

**Control** (`src/ESPUIcontrol.h`, `src/ESPUIcontrol.cpp`)
- Represents a single UI element (button, slider, label, etc.)
- Stored as a linked list (via `Control* next`)
- Contains: `type`, `id`, `label`, `value`, `color`, `callback`, visibility, styling
- Manages synchronization state with `ControlChangeID` to track when updates are needed
- Marshals itself to JSON for transmission to clients

**ESPUIclient** (`src/ESPUIclient.h`, `src/ESPUIclient.cpp`)
- Represents a single WebSocket client connection
- Implements state machine pattern for managing client sync state
- States: Idle, SendingUpdate, Rebuilding, Reloading (see `ESPUIclientFsm.h`)
- Handles chunked control transmission to avoid overwhelming clients
- Tracks sync state to determine if client needs updates, rebuilds, or reloads

**Client State Machine** (`src/ESPUIclientFsm.h`, `src/ESPUIclientFsm.cpp`)
- Four states manage client lifecycle:
  - **Idle**: Client is synchronized, no action needed
  - **SendingUpdate**: Sending incremental updates for changed controls
  - **Rebuilding**: Sending complete UI rebuild (all controls)
  - **Reloading**: Requesting client page reload
- Transitions based on control changes and client acknowledgments

### Communication Protocol

**WebSocket Message Flow**:
1. Client connects → Server sends initial GUI (all controls as JSON)
2. User interaction → Client sends control update → Server calls callback → Server broadcasts state to all clients
3. Server-side update → Server sends update message → Clients refresh affected controls

**Message Types** (defined in `ESPUI.h`):
- `InitialGui` (200): Complete UI definition sent to new clients
- `Reload` (201): Instructs client to reload the page
- `ExtendGUI` (210): Add new controls to existing UI
- `UpdateGui` (220): Update existing control values
- `ExtendedUpdateGui` (230): Update with additional metadata

### Control Synchronization

The library uses a change tracking system:
- Each control has a `ControlChangeID` timestamp
- Each client tracks its `CurrentSyncID`
- When a control changes, its `ControlChangeID` is updated
- Clients with `CurrentSyncID < ControlChangeID` need that control's update
- Prevents unnecessary retransmission of unchanged controls

### Memory Management

**ESP8266 Considerations**:
- Limited heap requires careful memory management
- Supports dual heap configuration (IRAM + DRAM) via `UMM_HEAP_IRAM`
- Use `HeapSelectIram` decorator for UI operations if needed
- Smaller JSON document sizes: `jsonInitialDocumentSize = 2000`
- JSON chunking with `jsonChunkNumberMax = 5`

**ESP32**:
- Larger default JSON documents: `jsonInitialDocumentSize = 8000`
- No chunking by default: `jsonChunkNumberMax = 0`
- Thread-safe control access via FreeRTOS semaphore

### Filesystem Modes

**PROGMEM (Default)**:
- Static assets embedded in flash memory as C headers
- No filesystem setup required
- Uses more program memory but works out of the box

**LittleFS**:
- Static assets stored in filesystem partition
- Call `ESPUI.prepareFileSystem()` once to populate LittleFS
- Then use `ESPUI.beginLITTLEFS()` instead of `ESPUI.begin()`
- Saves program memory for larger applications

## Control Types and Callbacks

All interactive controls accept callbacks with signature:
```cpp
void callback(Control* sender, int eventType)
```

Or extended callback with user parameter:
```cpp
void callback(Control* sender, int eventType, void* userParam)
```

**Event Types** (defined in `ESPUI.h`):
- Button: `B_DOWN`, `B_UP`
- Switcher: `S_ACTIVE`, `S_INACTIVE`
- Pad: `P_LEFT_DOWN`, `P_LEFT_UP`, `P_RIGHT_DOWN/UP`, `P_FOR_DOWN/UP`, `P_BACK_DOWN/UP`, `P_CENTER_DOWN/UP`
- Slider: `SL_VALUE`
- Number: `N_VALUE`
- Text: `T_VALUE`
- Select: `S_VALUE`
- Time: `TM_VALUE`

## Common Patterns

### Creating UI Controls
Controls can be created with simple API or full `addControl()`:
```cpp
// Simple API (no tab support)
uint16_t buttonId = ESPUI.button("Label", callback, ControlColor::Peterriver, "Value");

// Full API (supports tabs and parents)
uint16_t controlId = ESPUI.addControl(ControlType::Button, "Label", "Value",
                                       ControlColor::Peterriver, parentId, callback);
```

### Control Grouping
Controls can be grouped in the same panel by setting `parentControl`:
```cpp
auto panel = ESPUI.addControl(ControlType::Button, "Panel", "Button A", ControlColor::Turquoise,
                               Control::noParent, callback);
ESPUI.addControl(ControlType::Button, "", "Button B", ControlColor::None, panel, callback);
```

### Dynamic Updates
Update controls from server-side code:
```cpp
ESPUI.updateButton(buttonId, "New Text");
ESPUI.updateLabel(labelId, "New Value");
ESPUI.updateSlider(sliderId, newValue);
```

### Styling
Apply custom CSS to controls:
```cpp
ESPUI.setPanelStyle(controlId, "background-color: #FF0000;");
ESPUI.setElementStyle(controlId, "font-weight: bold;");
```

## Key Files

**Core Implementation**:
- `src/ESPUI.h` / `src/ESPUI.cpp` - Main library class
- `src/ESPUIcontrol.h` / `src/ESPUIcontrol.cpp` - Control representation
- `src/ESPUIclient.h` / `src/ESPUIclient.cpp` - Client connection management
- `src/ESPUIclientFsm.h` / `src/ESPUIclientFsm.cpp` - Client state machine

**Generated Assets** (auto-generated, do not edit directly):
- `src/dataIndexHTML.h` - Main HTML page
- `src/dataStyleCSS.h` / `src/dataNormalizeCSS.h` - CSS stylesheets
- `src/dataZeptoJS.h` / `src/dataControlsJS.h` / `src/dataSliderJS.h` / `src/dataTabbedcontentJS.h` / `src/dataGraphJS.h` - JavaScript code

**Source Assets** (edit these, then regenerate):
- `data/index.htm` - Main HTML template
- `data/css/` - CSS source files
- `data/js/` - JavaScript source files

**Examples**:
- `examples/gui/gui.ino` - Basic example with various controls
- `examples/tabbedGui/tabbedGui.ino` - Multi-tab interface example
- `examples/completeExample/completeExample.cpp` - Comprehensive feature demonstration
- `examples/completeLambda/completeLambda.ino` - Lambda callback examples
- `examples/prepareFilesystem/prepareFilesystem.ino` - LittleFS setup utility

## Dependencies

Required libraries:
- **ESPAsyncWebServer** - Async web server framework
- **ArduinoJson** (v6 or v7) - JSON serialization
- **AsyncTCP** (ESP32 only) - Async TCP library for ESP32
- **ESPAsyncTCP** (ESP8266 only) - Async TCP library for ESP8266
- **LittleFS** - Filesystem (built-in for ESP32, library for ESP8266)

## ArduinoJson Version Compatibility

The codebase supports both ArduinoJson v6 and v7 using preprocessor macros defined in `src/ESPUI.h`:
- v7: `JsonDocument`, dynamic arrays/objects via `to<>()`
- v6: `DynamicJsonDocument`, `createNestedArray()`, `createNestedObject()`

Macros handle API differences: `AllocateJsonDocument`, `AllocateJsonArray`, `AllocateJsonObject`.
