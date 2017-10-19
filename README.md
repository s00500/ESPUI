# ESPUI
![ESPUI Dashboard](https://github.com/s00500/ESPUI/blob/master/docs/img1.PNG)

ESPUI is a simple library to make a web user interface for your projects using the **ESP32**
It uses web sockets and lets you create, control, and update elements on your GUI through multiple devices like phones and tablets.

ESPUI uses simple Arduino-style syntax for creating a solid, functioning user interface without too much boilerplate code.

So if you either don't know how or just don't want to waste time: this is your simple solution user interface without the need of internet connectivity or any additional servers.

I completely rewrote the EasyUI Library created by ayushsharma82 [Here](https://github.com/ayushsharma82/)
Now it uses ESPAsyncWebserver and is mainly to be used with the ESP32 Processor.
ESP8266 support will be interesting maybe in the future.

## Getting started (Filesystem upload)

ESPUI **NEEDS** its files burnt on the SPIFFS filesystem on the ESP. **Without this ESPUI will NOT work at all**

To do this download and install me-no-devs wonderful [ESP32 sketch data uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin)

Then open the example sketch and select "ESP32 Upload Sketch Data" from the Tools menu.
Now you are set to go and use any code you want to with this library




## How to Install
###### Directly Through Arduino IDE

THIS IS NOT DONE YET
~~Go to Sketch > Include Library > Library Manager > Search for "EasyUI" > Install~~

###### Manual Install

For Windows: Download the [Repository](https://github.com/s00500/ESPUI/archive/master.zip) and extract the .zip in Documents>Arduino>Libraries>{Place "ESPUI" folder Here}

For Linux: Download the [Repository](https://github.com/s00500/ESPUI/archive/master.zip) and extract the .zip in Sketchbook/Libraries/{Place "ESPUI" folder Here}

For macOs: Download the [Repository](https://github.com/s00500/ESPUI/archive/master.zip) and extract the .zip in ~/Documents/Arduino/libraries/{Place "ESPUI" folder Here}

###### Manually through IDE

Download the [Repository](https://github.com/s00500/ESPUI/archive/master.zip), Go to Sketch>Include Library>Add .zip Library> Select the Downloaded .zip File.

## Dependencies
This library is dependent on the following libraries to function properly.
  - [ESP32 Core Library](https://github.com/espressif/arduino-esp32)
  - [ESPAsyncWebserver](https://github.com/me-no-dev/ESPAsyncWebServer)
  (There is still an open issue because there is a mistake in the current version that creates an error, please wait until that is done or checkout the [issue](https://github.com/me-no-dev/ESPAsyncWebServer/issues/234))
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)


## User interface Elements
  - Label (update able)
  - Button
  - Switch (update able)
  - Control pad
  - Control pad with center button

  Checkout the example for the usage

## Roadmap :

- Refactor id system
- make button colour customisable
- cleanup unnecessary CSS
- gzip files to make them load faster
- setup spiffs using values in program memory ? (if you have feedback to this idea let me know)
- Support ESP8266

## Documentation

The heart of ESPUI is
ESPUI's frontend is based on [Skeleton CSS](http://getskeleton.com/) and jQuery-like lightweight [zepto.js](https://zeptojs.com/) for Handling Click Events Etc. The communication between the *ESP32* and the client browser works using web sockets.
ESPUI does not need network access and can be used in standalone access point mode.
All assets are loaded from the internal SPIFFS filesystem of the ESP32.

# Contribute
Liked this Library? You can Support me by sending me a :coffee: [Coffee](https://paypal.me/lukasbachschwell/3).
