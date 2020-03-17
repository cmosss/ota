#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#include "cJSON.h"
#define USE_SERIAL Serial
void otaupdate(String T)
{
   if((WiFi.status() == WL_CONNECTED)) {
        t_httpUpdate_return ret = ESPhttpUpdate.update(T);
        switch(ret) {
            case HTTP_UPDATE_FAILED:
                USE_SERIAL.printf("HTTP_UPDATE_FAILD Error (%d): %s\r\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;
            case HTTP_UPDATE_NO_UPDATES:
                USE_SERIAL.println("HTTP_UPDATE_NO_UPDATES");
                break;
            case HTTP_UPDATE_OK:
                USE_SERIAL.println("HTTP_UPDATE_OK");
                break;
        }
    }
}

void check_Update()
{
   if((WiFi.status() == WL_CONNECTED)) 
   {
     HTTPClient https; 
       https.begin(UPDATE_JSON_URL);
       int httpCode = https.GET();
       Serial.println(httpCode);
           if (httpCode == 200) 
           {
                cJSON *json = cJSON_Parse(https.getString().c_str());
                //Serial.println(https.getString()+" TT");
                if(json == NULL)Serial.println("downloaded file is not a valid json, aborting...\n");
                else
                {
                        cJSON *version = cJSON_GetObjectItemCaseSensitive(json, "version");
                        cJSON *file = cJSON_GetObjectItemCaseSensitive(json, "file"); 
                        if(!cJSON_IsNumber(version))Serial.println("unable to read new version, aborting...\n");    
                        else
                        {
                          double new_version = version->valuedouble;
                          Serial.println(new_version);
                            if(new_version > FIRMWARE_VERSION) 
                            {  
                              Serial.println(file->valuestring);  
                              otaupdate(file->valuestring);    
                            }
                            }
                      }
           }
           else
           {
            Serial.print("AA_unable to download the json file, aborting...\n");   
           }
   }
}
