set(includedirs src)

set(req arduino-esp32 ESPAsyncWebServer ArduinoJson)

idf_component_register(
	INCLUDE_DIRS ${includedirs} 
	SRC_DIRS src
	REQUIRES ${req})
