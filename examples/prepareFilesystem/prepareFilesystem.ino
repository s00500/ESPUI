#include <ESPUI.h>

void setup(void)
{
  Serial.begin(115200);
  ESPUI.setVerbosity(Verbosity::Verbose); //Enable verbose output so you see the files in LittleFS
  delay(500); //Delay to allow Serial Monitor to start after a reset
  Serial.println(F("\nPreparing filesystem with ESPUI resources"));
  ESPUI.prepareFileSystem();  //Copy across current version of ESPUI resources
  Serial.println(F("Done, files..."));
  ESPUI.list(); //List all files on LittleFS, for info
}

void loop()
{
}
