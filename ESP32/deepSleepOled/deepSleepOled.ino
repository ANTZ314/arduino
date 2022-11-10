/***
 * ESP32: Deep Sleep & OLED Display
 * 
 * Note:
 * Val1 & Val2 are cleared on every sleep command
 * 
 * SDA - 21
 * SCL - 22
 */

#include "esp_deep_sleep.h"
#include <Wire.h>
#include "SSD1306.h" 

#define LED 2
int val1, val2 = 1;

// Initialise the OLED
SSD1306  display(0x3c, SDA, SCL);

/*****************************/
/*          SETUP            */
/*****************************/
void setup() {
    Serial.begin(115200);
    display.init();
    pinMode(LED,OUTPUT);

    /****************/
    /* Display OLED */
    /****************/
    digitalWrite(LED, HIGH);
    
    display.clear();
    display.drawString(0, 0,"VAL1: ");
    //display.println(val1,2);
    display.drawString(0, 20, String(val1));
    
    display.drawString(10, 10, "VAL2: ");
    //display.println(val2,2);
    display.drawString(0, 30,String(val2));
    
    display.display();

    
    delay(3000);
    display.clear();
    display.display();
    digitalWrite(LED, LOW);

    val1 = val1 + 1;
    val2 = val2 + 2;
    
    /******************************/
    /* Enter into Deep Sleep Mode */
    /******************************/
    esp_deep_sleep_enable_timer_wakeup(10000000);
    esp_deep_sleep_start();
}

/****************************/
/*          LOOP            */
/****************************/
void loop() {
  // Tasks performed on wake-up are in Setup
  // Should never come here

}
