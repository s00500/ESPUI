/**
 * @file completeExample.cpp
 * @author Ian Gray @iangray1000
 * 
 * This is an example GUI to show off all of the features of ESPUI. 
 * This can be built using the Arduino IDE, or PlatformIO.
 * 
 * ---------------------------------------------------------------------------------------
 * If you just want to see examples of the ESPUI code, jump down to the setUpUI() function
 * ---------------------------------------------------------------------------------------
 * 
 * When this program boots, it will load an SSID and password from the EEPROM. 
 * The SSID is a null-terminated C string stored at EEPROM addresses 0-31
 * The password is a null-terminated C string stored at EEPROM addresses 32-95.
 * If these credentials do not work for some reason, the ESP will create an Access 
 * Point wifi with the SSID HOSTNAME (defined below). You can then connect and use 
 * the controls on the "Wifi Credentials" tab to store credentials into the EEPROM.
 * 
 */

#include <Arduino.h>
#include <EEPROM.h>
#include <ESPUI.h>

#if defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
// esp8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <umm_malloc/umm_heap_select.h>
#ifndef CORE_MOCK
#ifndef MMU_IRAM_HEAP
#warning Try MMU option '2nd heap shared' in 'tools' IDE menu (cf. https://arduino-esp8266.readthedocs.io/en/latest/mmu.html#option-summary)
#warning use decorators: { HeapSelectIram doAllocationsInIRAM; ESPUI.addControl(...) ... } (cf. https://arduino-esp8266.readthedocs.io/en/latest/mmu.html#how-to-select-heap)
#warning then check http://<ip>/heap
#endif // MMU_IRAM_HEAP
#ifndef DEBUG_ESP_OOM
#error on ESP8266 and ESPUI, you must define OOM debug option when developping
#endif
#endif
#endif

//Settings
#define SLOW_BOOT 0
#define HOSTNAME "ESPUITest"
#define FORCE_USE_HOTSPOT 0


//Function Prototypes
void connectWifi();
void setUpUI();
void enterWifiDetailsCallback(Control *sender, int type);
void textCallback(Control *sender, int type);
void generalCallback(Control *sender, int type);
void scrambleCallback(Control *sender, int type);
void styleCallback(Control *sender, int type);
void updateCallback(Control *sender, int type);
void getTimeCallback(Control *sender, int type);
void graphAddCallback(Control *sender, int type);
void graphClearCallback(Control *sender, int type);
void randomString(char *buf, int len);
void extendedCallback(Control* sender, int type, void* param);

//UI handles
uint16_t wifi_ssid_text, wifi_pass_text;
uint16_t mainLabel, mainSwitcher, mainSlider, mainText, mainNumber, mainScrambleButton, mainTime;
uint16_t styleButton, styleLabel, styleSwitcher, styleSlider, styleButton2, styleLabel2, styleSlider2;
uint16_t graph;
volatile bool updates = false;



// This is the main function which builds our GUI
void setUpUI() {

#ifdef ESP8266
    { HeapSelectIram doAllocationsInIRAM;
#endif

	//Turn off verbose debugging
	ESPUI.setVerbosity(Verbosity::Quiet);

	//Make sliders continually report their position as they are being dragged.
	ESPUI.sliderContinuous = true;

	//This GUI is going to be a tabbed GUI, so we are adding most controls using ESPUI.addControl
	//which allows us to set a parent control. If we didn't need tabs we could use the simpler add
	//functions like:
	//    ESPUI.button()
	//    ESPUI.label()


	/*
	 * Tab: Basic Controls
	 * This tab contains all the basic ESPUI controls, and shows how to read and update them at runtime.
	 *-----------------------------------------------------------------------------------------------------------*/
	auto maintab = ESPUI.addControl(Control::Type::Tab, "", "Basic controls");

	ESPUI.addControl(Control::Type::Separator, "General controls", "", Control::Color::None, maintab);
	ESPUI.addControl(Control::Type::Button, "Button", "Button 1", Control::Color::Alizarin, maintab, extendedCallback, (void*)19);
	mainLabel = ESPUI.addControl(Control::Type::Label, "Label", "Label text", Control::Color::Emerald, maintab, generalCallback);
	mainSwitcher = ESPUI.addControl(Control::Type::Switcher, "Switcher", "", Control::Color::Sunflower, maintab, generalCallback);

	//Sliders default to being 0 to 100, but if you want different limits you can add a Min and Max control
	mainSlider = ESPUI.addControl(Control::Type::Slider, "Slider", "200", Control::Color::Turquoise, maintab, generalCallback);
	ESPUI.addControl(Control::Type::Min, "", "10", Control::Color::None, mainSlider);
	ESPUI.addControl(Control::Type::Max, "", "400", Control::Color::None, mainSlider);

	//These are the values for the selector's options. (Note that they *must* be declared static
	//so that the storage is allocated in global memory and not just on the stack of this function.)
	static String optionValues[] {"Value 1", "Value 2", "Value 3", "Value 4", "Value 5"};
	auto mainselector = ESPUI.addControl(Control::Type::Select, "Selector", "Selector", Control::Color::Wetasphalt, maintab, generalCallback);
	for(auto const& v : optionValues) {
		ESPUI.addControl(Control::Type::Option, v.c_str(), v, Control::Color::None, mainselector);
	}

	mainText = ESPUI.addControl(Control::Type::Text, "Text Input", "Initial value", Control::Color::Alizarin, maintab, generalCallback);

	//Number inputs also accept Min and Max components, but you should still validate the values.
	mainNumber = ESPUI.addControl(Control::Type::Number, "Number Input", "42", Control::Color::Emerald, maintab, generalCallback);
	ESPUI.addControl(Control::Type::Min, "", "10", Control::Color::None, mainNumber);
	ESPUI.addControl(Control::Type::Max, "", "50", Control::Color::None, mainNumber);

	ESPUI.addControl(Control::Type::Separator, "Updates", "", Control::Color::None, maintab);

	//This button will update all the updatable controls on this tab to random values
	mainScrambleButton = ESPUI.addControl(Control::Type::Button, "Scramble Values", "Scramble Values", Control::Color::Carrot, maintab, scrambleCallback);
	ESPUI.addControl(Control::Type::Switcher, "Constant updates", "0", Control::Color::Carrot, maintab, updateCallback);
	mainTime = ESPUI.addControl(Control::Type::Time, "", "", Control::Color::None, 0, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Get Time", "Get Time", Control::Color::Carrot, maintab, getTimeCallback);

	ESPUI.addControl(Control::Type::Separator, "Control Pads", "", Control::Color::None, maintab);
	ESPUI.addControl(Control::Type::Pad, "Normal", "", Control::Color::Peterriver, maintab, generalCallback);
	ESPUI.addControl(Control::Type::PadWithCenter, "With center", "", Control::Color::Peterriver, maintab, generalCallback);


	/*
	 * Tab: Colours
	 * This tab shows all the basic colours
	 *-----------------------------------------------------------------------------------------------------------*/
	auto colourtab = ESPUI.addControl(Control::Type::Tab, "", "Colours");
	ESPUI.addControl(Control::Type::Button, "Alizarin", "Alizarin", Control::Color::Alizarin, colourtab, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Turquoise", "Turquoise", Control::Color::Turquoise, colourtab, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Emerald", "Emerald", Control::Color::Emerald, colourtab, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Peterriver", "Peterriver", Control::Color::Peterriver, colourtab, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Wetasphalt", "Wetasphalt", Control::Color::Wetasphalt, colourtab, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Sunflower", "Sunflower", Control::Color::Sunflower, colourtab, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Carrot", "Carrot", Control::Color::Carrot, colourtab, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Dark", "Dark", Control::Color::Dark, colourtab, generalCallback);


	/*
	 * Tab: Styled controls
	 * This tab shows off how inline CSS styles can be applied to elements and panels in order
	 * to customise the look of the UI.
	 *-----------------------------------------------------------------------------------------------------------*/
	auto styletab = ESPUI.addControl(Control::Type::Tab, "", "Styled controls");
	styleButton = ESPUI.addControl(Control::Type::Button, "Styled Button", "Button", Control::Color::Alizarin, styletab, generalCallback);
	styleLabel = ESPUI.addControl(Control::Type::Label, "Styled Label", "This is a label", Control::Color::Alizarin, styletab, generalCallback);
	styleSwitcher = ESPUI.addControl(Control::Type::Switcher, "Styled Switcher", "1", Control::Color::Alizarin, styletab, generalCallback);
	styleSlider = ESPUI.addControl(Control::Type::Slider, "Styled Slider", "0", Control::Color::Alizarin, styletab, generalCallback);

	//This button will randomise the colours of the above controls to show updating of inline styles
	ESPUI.addControl(Control::Type::Button, "Randomise Colours", "Randomise Colours", Control::Color::Sunflower, styletab, styleCallback);

	ESPUI.addControl(Control::Type::Separator, "Other styling examples", "", Control::Color::None, styletab);
	styleButton2 = ESPUI.addControl(Control::Type::Button, "Styled Button", "Button", Control::Color::Alizarin, styletab, generalCallback);
	ESPUI.setPanelStyle(styleButton2, "background: linear-gradient(90deg, rgba(131,58,180,1) 0%, rgba(253,29,29,1) 50%, rgba(252,176,69,1) 100%); border-bottom: #555;");
	ESPUI.setElementStyle(styleButton2, "border-radius: 2em; border: 3px solid black; width: 30%; background-color: #8df;");

	styleSlider2 = ESPUI.addControl(Control::Type::Slider, "Styled Slider", "0", Control::Color::Dark, styletab, generalCallback);
	ESPUI.setElementStyle(styleSlider2, "background: linear-gradient(to right, red, orange, yellow, green, blue);");

	styleLabel2 = ESPUI.addControl(Control::Type::Label, "Styled Label", "This is a label", Control::Color::Dark, styletab, generalCallback);
	ESPUI.setElementStyle(styleLabel2, "text-shadow: 3px 3px #74b1ff, 6px 6px #c64ad7; font-size: 60px; font-variant-caps: small-caps; background-color: unset; color: #c4f0bb; -webkit-text-stroke: 1px black;");


	/*
	 * Tab: Grouped controls
	 * This tab shows how multiple control can be grouped into the same panel through the use of the
	 * parentControl value. This also shows how to add labels to grouped controls, and how to use vertical controls.
	 *-----------------------------------------------------------------------------------------------------------*/
	auto grouptab = ESPUI.addControl(Control::Type::Tab, "", "Grouped controls");

	//The parent of this button is a tab, so it will create a new panel with one control.
	auto groupbutton = ESPUI.addControl(Control::Type::Button, "Button Group", "Button A", Control::Color::Dark, grouptab, generalCallback);
	//However the parent of this button is another control, so therefore no new panel is
	//created and the button is added to the existing panel.
	ESPUI.addControl(Control::Type::Button, "", "Button B", Control::Color::Alizarin, groupbutton, generalCallback);
	ESPUI.addControl(Control::Type::Button, "", "Button C", Control::Color::Alizarin, groupbutton, generalCallback);


	//Sliders can be grouped as well
	//To label each slider in the group, we are going add additional labels and give them custom CSS styles
	//We need this CSS style rule, which will remove the label's background and ensure that it takes up the entire width of the panel
	String clearLabelStyle = "background-color: unset; width: 100%;";
	//First we add the main slider to create a panel
	auto groupsliders = ESPUI.addControl(Control::Type::Slider, "Slider Group", "10", Control::Color::Dark, grouptab, generalCallback);
	//Then we add a label and set its style to the clearLabelStyle. Here we've just given it the name "A"
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "A", Control::Color::None, groupsliders), clearLabelStyle);
	//We can now continue to add additional sliders and labels
	ESPUI.addControl(Control::Type::Slider, "", "20", Control::Color::None, groupsliders, generalCallback);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "B", Control::Color::None, groupsliders), clearLabelStyle);
	ESPUI.addControl(Control::Type::Slider, "", "30", Control::Color::None, groupsliders, generalCallback);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "C", Control::Color::None, groupsliders), clearLabelStyle);

	//We can also usefully group switchers.
	auto groupswitcher = ESPUI.addControl(Control::Type::Switcher, "Switcher Group", "0", Control::Color::Dark, grouptab, generalCallback);
	ESPUI.addControl(Control::Type::Switcher, "", "1", Control::Color::Sunflower, groupswitcher, generalCallback);
	ESPUI.addControl(Control::Type::Switcher, "", "0", Control::Color::Sunflower, groupswitcher, generalCallback);
	ESPUI.addControl(Control::Type::Switcher, "", "1", Control::Color::Sunflower, groupswitcher, generalCallback);
	//To label these switchers we need to first go onto a "new line" below the line of switchers
	//To do this we add an empty label set to be clear and full width (with our clearLabelStyle)
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "", Control::Color::None, groupswitcher), clearLabelStyle);
	//We will now need another label style. This one sets its width to the same as a switcher (and turns off the background)
	String switcherLabelStyle = "width: 60px; margin-left: .3rem; margin-right: .3rem; background-color: unset;";
	//We can now just add the styled labels.
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "A", Control::Color::None, groupswitcher), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "B", Control::Color::None, groupswitcher), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "C", Control::Color::None, groupswitcher), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "D", Control::Color::None, groupswitcher), switcherLabelStyle);

	//You can mix and match different control types, but the results might sometimes
	//need additional styling to lay out nicely.
	auto grouplabel = ESPUI.addControl(Control::Type::Label, "Mixed Group", "Main label", Control::Color::Dark, grouptab);
	auto grouplabel2 = ESPUI.addControl(Control::Type::Label, "", "Secondary label", Control::Color::Emerald, grouplabel);
	ESPUI.addControl(Control::Type::Button, "", "Button D", Control::Color::Alizarin, grouplabel, generalCallback);
	ESPUI.addControl(Control::Type::Switcher, "", "1", Control::Color::Sunflower, grouplabel, generalCallback);
	ESPUI.setElementStyle(grouplabel2, "font-size: x-large; font-family: serif;");

	//Some controls can even support vertical orientation, currently Switchers and Sliders
	ESPUI.addControl(Control::Type::Separator, "Vertical controls", "", Control::Color::None, grouptab);
	auto vertgroupswitcher = ESPUI.addControl(Control::Type::Switcher, "Vertical Switcher Group", "0", Control::Color::Dark, grouptab, generalCallback);
	ESPUI.setVertical(vertgroupswitcher); 
	//On the following lines we wrap the value returned from addControl and send it straight to setVertical
	ESPUI.setVertical(ESPUI.addControl(Control::Type::Switcher, "", "0", Control::Color::None, vertgroupswitcher, generalCallback));
	ESPUI.setVertical(ESPUI.addControl(Control::Type::Switcher, "", "0", Control::Color::None, vertgroupswitcher, generalCallback));
	ESPUI.setVertical(ESPUI.addControl(Control::Type::Switcher, "", "0", Control::Color::None, vertgroupswitcher, generalCallback));
	//The mechanism for labelling vertical switchers is the same as we used above for horizontal ones
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "", Control::Color::None, vertgroupswitcher), clearLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "A", Control::Color::None, vertgroupswitcher), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "B", Control::Color::None, vertgroupswitcher), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "C", Control::Color::None, vertgroupswitcher), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "D", Control::Color::None, vertgroupswitcher), switcherLabelStyle);

	auto vertgroupslider = ESPUI.addControl(Control::Type::Slider, "Vertical Slider Group", "15", Control::Color::Dark, grouptab, generalCallback);
	ESPUI.setVertical(vertgroupslider);
	ESPUI.setVertical(ESPUI.addControl(Control::Type::Slider, "", "25", Control::Color::None, vertgroupslider, generalCallback));
	ESPUI.setVertical(ESPUI.addControl(Control::Type::Slider, "", "35", Control::Color::None, vertgroupslider, generalCallback));
	ESPUI.setVertical(ESPUI.addControl(Control::Type::Slider, "", "45", Control::Color::None, vertgroupslider, generalCallback));
	//The mechanism for labelling vertical sliders is the same as we used above for switchers
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "", Control::Color::None, vertgroupslider), clearLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "A", Control::Color::None, vertgroupslider), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "B", Control::Color::None, vertgroupslider), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "C", Control::Color::None, vertgroupslider), switcherLabelStyle);
	ESPUI.setElementStyle(ESPUI.addControl(Control::Type::Label, "", "D", Control::Color::None, vertgroupslider), switcherLabelStyle);

	//Note that combining vertical and horizontal sliders is going to result in very messy layout!

	/*
	 * Tab: Example UI
	 * An example UI for the documentation
	 *-----------------------------------------------------------------------------------------------------------*/
	auto exampletab = ESPUI.addControl(Control::Type::Tab, "Example", "Example");
	ESPUI.addControl(Control::Type::Separator, "Control and Status", "", Control::Color::None, exampletab);
	ESPUI.addControl(Control::Type::Switcher, "Power", "1", Control::Color::Alizarin, exampletab, generalCallback);
	ESPUI.addControl(Control::Type::Label, "Status", "System status: OK", Control::Color::Wetasphalt, exampletab, generalCallback);
	
	ESPUI.addControl(Control::Type::Separator, "Settings", "", Control::Color::None, exampletab);
	ESPUI.addControl(Control::Type::PadWithCenter, "Attitude Control", "", Control::Color::Dark, exampletab, generalCallback);
	auto examplegroup1 = ESPUI.addControl(Control::Type::Button, "Activate Features", "Feature A", Control::Color::Carrot, exampletab, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Activate Features", "Feature B", Control::Color::Carrot, examplegroup1, generalCallback);
	ESPUI.addControl(Control::Type::Button, "Activate Features", "Feature C", Control::Color::Carrot, examplegroup1, generalCallback);
	ESPUI.addControl(Control::Type::Slider, "Value control", "45", Control::Color::Peterriver, exampletab, generalCallback);

	/*
	 * Tab: WiFi Credentials
	 * You use this tab to enter the SSID and password of a wifi network to autoconnect to.
	 *-----------------------------------------------------------------------------------------------------------*/
	auto wifitab = ESPUI.addControl(Control::Type::Tab, "", "WiFi Credentials");
	wifi_ssid_text = ESPUI.addControl(Control::Type::Text, "SSID", "", Control::Color::Alizarin, wifitab, textCallback);
	//Note that adding a "Max" control to a text control sets the max length
	ESPUI.addControl(Control::Type::Max, "", "32", Control::Color::None, wifi_ssid_text);
	wifi_pass_text = ESPUI.addControl(Control::Type::Text, "Password", "", Control::Color::Alizarin, wifitab, textCallback);
	ESPUI.addControl(Control::Type::Max, "", "64", Control::Color::None, wifi_pass_text);
	ESPUI.addControl(Control::Type::Button, "Save", "Save", Control::Color::Peterriver, wifitab, enterWifiDetailsCallback);


	//Finally, start up the UI. 
	//This should only be called once we are connected to WiFi.
	ESPUI.begin(HOSTNAME);

#ifdef ESP8266
    } // HeapSelectIram
#endif

}

//This callback generates and applies inline styles to a bunch of controls to change their colour.
//The styles created are of the form: 
//	"border-bottom: #999 3px solid; background-color: #aabbcc;"
//	"background-color: #aabbcc;"
void styleCallback(Control *sender, int type) {
	//Declare space for style strings. These have to be static so that they are always available
	//to the websocket layer. If we'd not made them static they'd be allocated on the heap and
	//will be unavailable when we leave this function.
	static char stylecol1[60], stylecol2[30]; 
	if(type == B_UP) {
		//Generate two random HTML hex colour codes, and print them into CSS style rules
		sprintf(stylecol1, "border-bottom: #999 3px solid; background-color: #%06X;", (unsigned int) random(0x0, 0xFFFFFF));
		sprintf(stylecol2, "background-color: #%06X;", (unsigned int) random(0x0, 0xFFFFFF));

		//Apply those styles to various elements to show how controls react to styling
		ESPUI.setPanelStyle(styleButton, stylecol1);
		ESPUI.setElementStyle(styleButton, stylecol2);
		ESPUI.setPanelStyle(styleLabel, stylecol1);
		ESPUI.setElementStyle(styleLabel, stylecol2);
		ESPUI.setPanelStyle(styleSwitcher, stylecol1);
		ESPUI.setElementStyle(styleSwitcher, stylecol2);
		ESPUI.setPanelStyle(styleSlider, stylecol1);
		ESPUI.setElementStyle(styleSlider, stylecol2);
	}
}


//This callback updates the "values" of a bunch of controls
void scrambleCallback(Control *sender, int type) {
	static char rndString1[10];
	static char rndString2[20];
	static bool scText = false;

	if(type == B_UP) { //Button callbacks generate events for both UP and DOWN.
		//Generate some random text
		randomString(rndString1, 10);
		randomString(rndString2, 20);

		//Set the various controls to random value to show how controls can be updated at runtime
		ESPUI.updateLabel(mainLabel, String(rndString1));
		ESPUI.updateSwitcher(mainSwitcher, ESPUI.getControl(mainSwitcher)->value.toInt() ? false : true);
		ESPUI.updateSlider(mainSlider, random(10, 400));
		ESPUI.updateText(mainText, String(rndString2));
		ESPUI.updateNumber(mainNumber, random(100000));
		ESPUI.updateButton(mainScrambleButton, scText ? "Scrambled!" : "Scrambled.");
		scText = !scText;
	}
}

void updateCallback(Control *sender, int type) {
	updates = (sender->value.toInt() > 0);
}

void getTimeCallback(Control *sender, int type) {
	if(type == B_UP) {
		ESPUI.updateTime(mainTime);
	}
}

void graphAddCallback(Control *sender, int type) {
	if(type == B_UP) {
		ESPUI.addGraphPoint(graph, random(1, 50));
	}
}

void graphClearCallback(Control *sender, int type) {
	if(type == B_UP) {
		ESPUI.clearGraph(graph);
	}
}


//Most elements in this test UI are assigned this generic callback which prints some
//basic information. Event types are defined in ESPUI.h
void generalCallback(Control *sender, int type) {
	Serial.print("CB: id(");
	Serial.print(sender->id);
	Serial.print(") Type(");
	Serial.print(type);
	Serial.print(") '");
	Serial.print(sender->label);
	Serial.print("' = ");
	Serial.println(sender->value);
}

// Most elements in this test UI are assigned this generic callback which prints some
// basic information. Event types are defined in ESPUI.h
// The extended param can be used to hold a pointer to additional information
// or for C++ it can be used to return a this pointer for quick access
// using a lambda function
void extendedCallback(Control* sender, int type, void* param)
{
    Serial.print("CB: id(");
    Serial.print(sender->id);
    Serial.print(") Type(");
    Serial.print(type);
    Serial.print(") '");
    Serial.print(sender->label);
    Serial.print("' = ");
    Serial.println(sender->value);
    Serial.print("param = ");
    Serial.println((long)param);
}

void setup() {
	randomSeed(0);
	Serial.begin(115200);
	while(!Serial);
	if(SLOW_BOOT) delay(5000); //Delay booting to give time to connect a serial monitor
	connectWifi();
	#if defined(ESP32)
		WiFi.setSleep(false); //For the ESP32: turn off sleeping to increase UI responsivness (at the cost of power use)
	#endif
	setUpUI();
}

void loop() {
	static long unsigned lastTime = 0;

	//Send periodic updates if switcher is turned on
	if(updates && millis() > lastTime + 500) {
		static uint16_t sliderVal = 10;

		//Flick this switcher on and off
		ESPUI.updateSwitcher(mainSwitcher, ESPUI.getControl(mainSwitcher)->value.toInt() ? false : true);
		sliderVal += 10;
		if(sliderVal > 400) sliderVal = 10;

		//Sliders, numbers, and labels can all be updated at will
		ESPUI.updateSlider(mainSlider, sliderVal);
		ESPUI.updateNumber(mainNumber, random(100000));
		ESPUI.updateLabel(mainLabel, String(sliderVal));
		lastTime = millis();
	}

	//Simple debug UART interface
	if(Serial.available()) {
		switch(Serial.read()) {
			case 'w': //Print IP details
				Serial.println(WiFi.localIP());
				break;
			case 'W': //Reconnect wifi
				connectWifi();
				break;
			case 'C': //Force a crash (for testing exception decoder)
				#if !defined(ESP32)
					((void (*)())0xf00fdead)();
				#endif
				break;
			default:
				Serial.print('#');
				break;
		}
	}

	#if !defined(ESP32)
		//We don't need to call this explicitly on ESP32 but we do on 8266
		MDNS.update();
	#endif

}




//Utilities
//
//If you are here just to see examples of how to use ESPUI, you can ignore the following functions
//------------------------------------------------------------------------------------------------
void readStringFromEEPROM(String& buf, int baseaddress, int size) {
	buf.reserve(size);
	for (int i = baseaddress; i < baseaddress+size; i++) {
		char c = EEPROM.read(i);
		buf += c;
		if(!c) break;
	}	
}

void connectWifi() {
	int connect_timeout;

#if defined(ESP32)
	WiFi.setHostname(HOSTNAME);
#else
	WiFi.hostname(HOSTNAME);
#endif
	Serial.println("Begin wifi...");

	//Load credentials from EEPROM 
	if(!(FORCE_USE_HOTSPOT)) {
		yield();
		EEPROM.begin(100);
		String stored_ssid, stored_pass;
		readStringFromEEPROM(stored_ssid, 0, 32);
		readStringFromEEPROM(stored_pass, 32, 96);
		EEPROM.end();
	
		//Try to connect with stored credentials, fire up an access point if they don't work.
		#if defined(ESP32)
			WiFi.begin(stored_ssid.c_str(), stored_pass.c_str());
		#else
			WiFi.begin(stored_ssid, stored_pass);
		#endif
		connect_timeout = 28; //7 seconds
		while (WiFi.status() != WL_CONNECTED && connect_timeout > 0) {
			delay(250);
			Serial.print(".");
			connect_timeout--;
		}
	}
	
	if (WiFi.status() == WL_CONNECTED) {
		Serial.println(WiFi.localIP());
		Serial.println("Wifi started");

		if (!MDNS.begin(HOSTNAME)) {
			Serial.println("Error setting up MDNS responder!");
		}
	} else {
		Serial.println("\nCreating access point...");
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
		WiFi.softAP(HOSTNAME);

		connect_timeout = 20;
		do {
			delay(250);
			Serial.print(",");
			connect_timeout--;
		} while(connect_timeout);
	}
}

void enterWifiDetailsCallback(Control *sender, int type) {
	if(type == B_UP) {
		Serial.println("Saving credentials to EPROM...");
		Serial.println(ESPUI.getControl(wifi_ssid_text)->value);
		Serial.println(ESPUI.getControl(wifi_pass_text)->value);
		unsigned int i;
		EEPROM.begin(100);
		for(i = 0; i < ESPUI.getControl(wifi_ssid_text)->value.length(); i++) {
			EEPROM.write(i, ESPUI.getControl(wifi_ssid_text)->value.charAt(i));
			if(i==30) break; //Even though we provided a max length, user input should never be trusted
		}
		EEPROM.write(i, '\0');

		for(i = 0; i < ESPUI.getControl(wifi_pass_text)->value.length(); i++) {
			EEPROM.write(i + 32, ESPUI.getControl(wifi_pass_text)->value.charAt(i));
			if(i==94) break; //Even though we provided a max length, user input should never be trusted
		}
		EEPROM.write(i + 32, '\0');
		EEPROM.end();
	}
}

void textCallback(Control *sender, int type) {
	//This callback is needed to handle the changed values, even though it doesn't do anything itself.
}

void randomString(char *buf, int len) {
	for(auto i = 0; i < len-1; i++) 
		buf[i] = random(0, 26) + 'A';
	buf[len-1] = '\0';
}
