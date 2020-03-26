#define FIRMWARE_VERSION  0.4
#define UPDATE_JSON_URL  "https://raw.githubusercontent.com/cmosss/ota/master/data_package.json"
#include "Root_wifi.h"
#include "Root_OTA.h"
void setup()
{
 Serial.begin(115200);   
 wifisetup();
 Task_OTA();
} 
void loop() 
{
  
}
