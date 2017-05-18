# EasyUI
Did you Ever Got into the Trouble of Making a good Looking UI for ESP8266 without having the skills to Build Perfect Webpages?
EasyUI is an User Interface Library for ESP8266 to Solve this Problem. This Library Uses Light-weight Websockets Protocol for Communicating with Webpage to Control, Make and Update Elements.

EasyUI uses functions native to arduino for creating the perfect Good Looking User Interface without the Need of Knowing Complex Javascripts etc.


  - Type some Markdown on the left
  - See HTML in the right
  - Magic

## How to Install
###### Directly Through Arduino IDE
Go to Sketch > Include Library > Library Manager > Search for "EasyUI" > Install

###### Mannual Install

For Windows: Download the [Repository](https://github.com/ayushsharma82/EasyUI/archive/master.zip) and extract the .zip in Documents>Arduino>Libraries>{Place "EasyUI" folder Here}

For Linux: Download the [Repository](https://github.com/ayushsharma82/EasyUI/archive/master.zip) and extract the .zip in Sketchbook>Libraries>{Place "EasyUI" folder Here}

###### Mannually through IDE

Download the [Repository](https://github.com/ayushsharma82/EasyUI/archive/master.zip), Go to Sketch>Include Library>Add .zip Library> Select the Downloaded .zip File.

## Dependancies
This Library is Dependent on the Following Libraries to Function Properly.
  - [ESP8266 Core Library](https://github.com/esp8266/Arduino)
  - [arduinoWebSockets Library](https://github.com/Links2004/arduinoWebSockets)
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

Make Sure all Libraries are Installed to make this Work.

## Features

EasyUI Currently has the Following User Interface Features:

  - Toggle Button
  - Label

Upcoming:
- ~~Toggle Button~~
- ~~Label~~
- Smart Variable
- Progress Bar
- Click Button
- DropBox
- Forums
- Tables

## Documentation

EasyUI is Based on [Skeleton CSS](http://getskeleton.com/) and Jquery for Handling Click Events Etc. The Communication Between ESP8266 and Webpage is with Websockets.
EasyUI is not Internet Dependent and will Continue working without any Internet Connection, All Assets are Loaded form ESP8266 Program Memory.

##### Following Functions can be Used in Sketch:
<br>
**To Set an Title for your Webpage:**
This Line of Code will Add your Custom Title to the Webpage Displayed by ESP8266. By Default is "EasyUI".
```
EasyUI.title("");
```
<br>
**To Make Label on Webpage:**
This will add an Label on your Web Interface.
```
EasyUI.label("Title", "Value");
```
<br>
**To Make Toggle Button on Webpage:**
This will add an toggle Button on your Webpage. For Extra Functionality* See Below
```
EasyUI.toggleButton(pin,"Title");
```
<br>
**Start the Library:**
Once you have Specified Any of the above Elements, Use this Below them to Start Inializing the Library.
```
EasyUI.begin();
```
<br>
**Loop Function:**
Don't Forget to Add this in your void loop()
```
EasyUI.loop();
```
<br>

## Extra Functionality
There are Some Elements in Library which have Added extra Functionality for Ease of Use.

**For Toggle Buttons:**
For Toggle Buttons There are 2 Extra Functionalities which can be defined by user.

- First is **{Start State}** , This is for Selecting if you want that **GPIO** to Start in **LOW** or **HIGH**. **By Default it's LOW.** (Define LOW or HIGH instead of '{Start State}' )

- Second **{Swap State}**, is useful for Swapping the States at which UI Buttons will work. making **'{Swap State}'** replace with **true** will make GPIO **LOW when Clicked "Turn On"** and **HIGH when clicked "Turn Off"**. By Default this is **false**.
```
EasyUI.toggleButton(pin, "Title", {Start State}, {Swap State});
```
# Contribute
Liked this Library? You can Support me by sending me a :coffee: [Coffee](https://www.paypal.me/ayushsharma82/3).
