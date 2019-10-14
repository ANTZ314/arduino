/***
 * ThingSpeak data update Test
 * DEFAULT I2C PINS:
 * 21 - SDA
 * 22 - SCL
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wifi.h>
#include "esp_deep_sleep.h"
#include <Wire.h>
#include "SSD1306.h" 
#include <Preferences.h>  //For reading and writing into the ROM memory

#define LED 2

Preferences preferences;

unsigned int counter;

// Initialise the OLED
SSD1306  display(0x3c, SDA, SCL);

HTTPClient http;   
 
char ssid[] = "Pretty_Fly_For_A_WiFi";         //  your network SSID (name) 
char pass[] = "#Mexicana411";   // your network password

char ssid1[]= "antz";
char pass1[] = "hkoNMhz";   // your network password


int status = WL_IDLE_STATUS;
 WiFiClient  client;
uint64_t gap=15*1000000;
unsigned long myChannelNumber = 279012;

float h,t = 1;
String poststr;
int httpCode,i = 0;

uint8_t board_farenheit;
float board_celsius;

esp_err_t esp32;

 void setup() 
 {
    Serial.begin(115200);
    display.init();
    pinMode(LED,OUTPUT);
    
    wifi1();

    digitalWrite(LED, HIGH);
    delay(3000);
    digitalWrite(LED, LOW);
    delay(3000);
    
    /*****************************/
    /* Display on Serial Monitor */  
    /*****************************/   
    Serial.print(t,2);
    Serial.print(",");
    Serial.print(h,2);
    Serial.println(",");


    /****************/
    /* Display OLED */
    /****************/
    display.clear();
    display.drawString(0, 0,"Temp (C):");
    display.println(t,2);
    display.drawString(10, 0, "Hdty (%):");
    display.println(h,2);
    display.display();

    
    t = t+1;    // Temporary increment value
    h = h+2;    // Temporary increment value
    
    /******************************/
    /* Enter into Deep Sleep Mode */
    /******************************/
    //esp_deep_sleep_enable_timer_wakeup(10000000);
    //esp_deep_sleep_start();
  
}

void loop() 
{
    digitalWrite(LED, HIGH);
    delay(3000);
    digitalWrite(LED, LOW);
    delay(3000);

}


void wifi1() 
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.print(".");
    }
    Serial.print("CONNECTED!");
}