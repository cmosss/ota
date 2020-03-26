#include <EEPROM.h>
int mode;
String ssid     = "";
String password = "";
void wifidataWrite(int B_mode,String ssid,String pass)
{
 String ROM=ssid+pass;
  EEPROM.write(0,B_mode);
  EEPROM.write(1,ssid.length());
  EEPROM.writeString(2,ROM);
  EEPROM.commit();
}
 void wifidataRead()
  {
    mode=EEPROM.read(0);
    int L_ssid= EEPROM.read(1);
    String ROM= EEPROM.readString(2);
    ssid=ROM.substring(0,L_ssid);
    password=ROM.substring(L_ssid);
  }
