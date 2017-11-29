# ESPUI
![ESPUI](https://github.com/s00500/ESPUI/blob/master/docs/ui_complete.png)


ESPUI is a simple library to make a web user interface for your projects using the **ESP32**
It uses web sockets and lets you create, control, and update elements on your GUI through multiple devices like phones and tablets.

ESPUI uses simple Arduino-style syntax for creating a solid, functioning user interface without too much boilerplate code.

So if you either don't know how or just don't want to waste time: this is your simple solution user interface without the need of internet connectivity or any additional servers.

I completely rewrote the EasyUI Library created by ayushsharma82 [Here](https://github.com/ayushsharma82/)
Now it uses ESPAsyncWebserver and is mainly to be used with the ESP32 Processor.
ESP8266 support will be interesting maybe in the future.

## Dependencies
This library is dependent on the following libraries to function properly.
  - [ESP32 Core Library](https://github.com/espressif/arduino-esp32)
  - [ESPAsyncWebserver](https://github.com/me-no-dev/ESPAsyncWebServer)
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)


## How to Install

Make sure all the dependencies are installed, then install like so:

#### Directly Through Arduino IDE

You can find this Library in the Arduino IDE library manager
Go to Sketch > Include Library > Library Manager > Search for "EasyUI" > Install

#### Manual Install

For Windows: Download the [Repository](https://github.com/s00500/ESPUI/archive/master.zip) and extract the .zip in Documents>Arduino>Libraries>{Place "ESPUI" folder Here}

For Linux: Download the [Repository](https://github.com/s00500/ESPUI/archive/master.zip) and extract the .zip in Sketchbook/Libraries/{Place "ESPUI" folder Here}

For macOs: Download the [Repository](https://github.com/s00500/ESPUI/archive/master.zip) and extract the .zip in ~/Documents/Arduino/libraries/{Place "ESPUI" folder Here}

#### Manually through IDE

Download the [Repository](https://github.com/s00500/ESPUI/archive/master.zip), Go to Sketch>Include Library>Add .zip Library> Select the Downloaded .zip File.

## Getting started (Filesystem upload)

ESPUI **NEEDS** its files burnt on the SPIFFS filesystem on the ESP. **Without this ESPUI will NOT work at all**

To do this download and install me-no-devs wonderful [ESP32 sketch data uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin)

Then open the example sketch and select "ESP32 Upload Sketch Data" from the Tools menu.
Now you are set to go and use any code you want to with this library

## User interface Elements
  - Label (updateable)
  - Button
  - Switch (updateable)
  - Control pad
  - Control pad with center button
  - Slider

  Checkout the example for the usage

## Roadmap :

- Document slider
- setup spiffs using values in program memory ? (if you have feedback to this idea let me know)
- ESP8266 support

## Documentation

The heart of ESPUI is [ESPAsyncWebserver](https://github.com/me-no-dev/ESPAsyncWebServer).
ESPUI's frontend is based on [Skeleton CSS](http://getskeleton.com/) and jQuery-like lightweight [zepto.js](https://zeptojs.com/) for Handling Click Events Etc. The communication between the *ESP32* and the client browser works using web sockets.
ESPUI does not need network access and can be used in standalone access point mode.
All assets are loaded from the internal SPIFFS filesystem of the ESP32.

This section will explain in detail how the Library is to be used from the Arduino code side. As of now the Facilino blocks are not implemented.
In the arduino setup() routine the interface can be customised by adding UI Elements. This is done by calling the corresponding library methods on the Library object ESPUI. Eg: ESPUI.button(“button”, &myCallback); creates a button in the interface that calls the “my-Callback” function when changed. All buttons and items call their callback whenever there is a state change from them. This means the button will call the callback when it is pressed and also again when it is released. To seperate different events an integer number with the event name is passed to the callback function that can be handled in a switch(){}case{} statement. Here is an overview of the currently implemented different elements of the UI library:


#### Button

![Buttons](https://github.com/s00500/ESPUI/blob/master/docs/ui_button.png)

Buttons have a name and a callback value. They have one event for press and one for release.


#### Switch

![Switches](https://github.com/s00500/ESPUI/blob/master/docs/ui_switches.png)

Switches sync their state on all connected devices. This means when you change their value they change visibly on all tablets or computers that currently display the interface. They also have two types of events: one for turning on and one for turning off.


#### Buttonpad

![control pads](https://github.com/s00500/ESPUI/blob/master/docs/ui_controlpad.png)

Button pads come in two flavours: with or without a center button. They are very useful for con-trolling all kinds of movements of vehicles or also of course our walking robots. They use a single callback per pad and have 8 or 10 different event types to differentiate the button actions.


#### Labels

![labels](https://github.com/s00500/ESPUI/blob/master/docs/ui_labels.png)

Labels are a nice tool to get information from the robot to the user interface. This can be done to show states, values of sensors and configuration parameters. To send data from the code use ESP.print(labelId, “Text”); . Labels get a name on creation and a initial value. The name is not changeable once the UI initialized.

#### Slider

There is also an slider component now, needs to be documented though

#### Initialization of the UI

After all the elements are configured you can use ESPUI.begin(“Some Title”); to start the UI interface. The web interface can then be used from multiple devices at once and also shows an connection status in the top bar.
The library is designed to be easy to use and can still be extended with a lot of more functionality.


# Contribute
Liked this Library? You can **support** me by sending me a :coffee: [Coffee](https://paypal.me/lukasbachschwell/3).
