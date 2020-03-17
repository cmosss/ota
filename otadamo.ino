#include <WiFi.h>
#define FIRMWARE_VERSION  0.4
#define UPDATE_JSON_URL  "http://192.168.0.101:1880/UUU"
#include "ota.h"
#define USE_SERIAL Serial 
 void check_update_task(void *pvParameter) 
 {
   while(1)
   {
      check_Update();
      vTaskDelay(60000/portTICK_PERIOD_MS);
   }
 }
 #define BLINK_GPIO      GPIO_NUM_5
 void blink_task(void *pvParameter) 
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void setup() 
  {
     Serial.begin(115200);
     WiFi.begin("cmos", "0810399997");
     while (WiFi.status() != WL_CONNECTED) 
     {
        delay(500);
        Serial.print(".");
      }
     Serial.println(WiFi.localIP());
     xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
     xTaskCreate(&check_update_task, "check_update_task", 8192, NULL, 5, NULL);
  }

void loop()
  {
  
  }
