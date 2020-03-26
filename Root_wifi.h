#include <Arduino.h>
#include "Root_EEPROM.h"
#include <WiFi.h>
#include <WebServer.h>
WebServer server(80);
String _page(String A ,String B)
{
   String MAIN1 PROGMEM = R"=====(
        <!DOCTYPE html>
        <html>
        <body>
        <h2>Wifi config<h2>
        <h3>
        <form action="/action_page">
        SSID:<br>
        <input type="text" name="ssid" value=")====="; 
   String MAIN2 PROGMEM = R"=====(">
        <br>
        Password :<br>
        <input type="text" name="pass" value=")=====";   
   String MAIN3  PROGMEM = R"=====(">
        <br><br>
        <input type="submit" value="Submit">
      </form> 
      </body>
      </html>
      )====="; 
String F = MAIN1+A+MAIN2+B+MAIN3;
return(F);     
}
void handleRoot() 
{
 server.send(200, "text/html",_page("","")); //Send web page
}

void handleForm()
{
 String _ssid = server.arg("ssid"); 
 String _pass = server.arg("pass"); 
 if( _ssid==""||_pass=="")
 {
 server.send(200, "text/html", _page(_ssid,_pass)); 
 }
 else
 {
 Serial.print("First Name:");
 Serial.println(_ssid);
 Serial.print("Last Name:");
 Serial.println(_pass); 
 wifidataWrite(1,_ssid,_pass);
 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page
 ESP.restart();
 }
}
//----------------------------------------------------
bool wifi_mode;
//WiFi.mode(WIFI_AP_STA);
void wifiAP_mode()
{
  wifi_mode=0;
  WiFi.mode(WIFI_AP);  
  WiFi.softAP("ESP32 AP mode",NULL);
}
void wifiSTA_mode()
{
  wifi_mode=1;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
}

#define SW_AP_MODE 2
#define LEDconnex 16
void blink_task(void *pvParameter)
{
int stepMamory=0;
int BUTT_SW_AP=0;
bool stat=0;
bool wifi_stat=0;
while(1)
  { 
        switch (stepMamory) 
        {
        case 0:
               Serial.println("0");
               EEPROM.begin(512); 
               wifidataRead();
               stepMamory++;
               pinMode(SW_AP_MODE,INPUT_PULLUP);
               pinMode(LEDconnex,OUTPUT);
               stat=(ssid!="")&&(BUTT_SW_AP==0);
               break;
        case 1:
                Serial.printf("0 -->%d\r\n",stat);
                
                if(stat)
                {
                  wifiSTA_mode();
                }
                else
                {
                     wifiAP_mode();
                     server.on("/", handleRoot);      //Which routine to handle at root location
                     server.on("/action_page", handleForm); //form action is handled here
                     server.begin();                  //Start server
                }
                  stepMamory++;   
                  break;
        case 2:
           Serial.printf("2 -->%d\r\n",BUTT_SW_AP);
           if(!digitalRead(SW_AP_MODE))BUTT_SW_AP++;
           else BUTT_SW_AP=0;
           if(BUTT_SW_AP>=6)
            {
              stepMamory=1;
              stat=0; 
            }   
                 bool _wifi_stat=(WiFi.status() == WL_CONNECTED);
                 if(_wifi_stat!=wifi_stat)
                 {
                   Serial.print(_wifi_stat);
                  Serial.print("--->");
                  if(_wifi_stat==0)
                  {
                    stepMamory=1;
                  }
                   Serial.println(WiFi.localIP());
                   digitalWrite(LEDconnex,_wifi_stat);
                  wifi_stat=_wifi_stat;
            }
        break;
       }
       if(wifi_mode==0)
       {
         server.handleClient(); 
       }
     vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void wifisetup()
{
 xTaskCreate(&blink_task, "blink_task", 8192, NULL, 5, NULL);
}

 
