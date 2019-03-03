#include <ESPUI.h>

ESPUIClass ESPUI( Verbosity::VerboseJSON );

void setup(void) {
  Serial.begin(115200);
  ESPUI.prepareFileSystem();
}

void loop() {
}
