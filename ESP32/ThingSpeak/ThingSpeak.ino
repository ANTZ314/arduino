/***
 * ThingSpeak data update Test
 * -> Powers Up, and attempts to connect to WiFi
 * -> Once Connected, retrieves IP address
 * -> Displays "Connected" & "IP Address" on OLED
 * -> Goes into Deep Sleep Mode for 20 seconds
 * -> On wake-up -> repeats
 * 
 * DEFAULT I2C PINS:
 * 21 - SDA
 * 22 - SCL
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wifi.h>
#include "esp_deep_sleep.h"
#include <Wire.h>         // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h"      // alias for `#include "SSD1306Wire.h"
#include <Preferences.h>  //For reading and writing into the ROM memory

// Initialize the OLED display using i2c
#define SDA 21
#define SCL 22

#define button 4
#define led 2

Preferences preferences;
unsigned int counter;

// Initialize the OLED display using Wire library
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
int BTN = 0;
String IP = "word";

uint8_t board_farenheit;
float board_celsius;

esp_err_t esp32;

void setup() 
{
    Serial.begin(115200);
    pinMode(19,OUTPUT);
    pinMode(led,OUTPUT);
    pinMode(button,INPUT);

    display.init();
    //display.flipScreenVertically();
    //display.setFont(ArialMT_Plain_10);

    preferences.begin("my-app", false);

    i=0;

    counter = preferences.getUInt("counter", 0);
    Serial.printf("Current counter value: %u\n", counter);

    if(counter==0 or counter==1)   WiFi.begin(ssid, pass);
    if(counter==2)   WiFi.begin(ssid1, pass1);
     
     while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        i=i+1;
        Serial.print(i);
        if(i>=40) {
    if(counter==0 or counter==1)   preferences.putUInt("counter", 2);
    if(counter==2)   preferences.putUInt("counter", 1);
      preferences.end();    
          fflush(stdout);
          esp_restart();     //Restart ESP if it does not get connected for long
        }
     }
     Serial.printf("CONNECTED!");

    Serial.println(WiFi.localIP());
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
    //display.setTextSize(2);
    display.drawString(10, 0, "CONNECTED");
    display.display();    
    delay(3000);
    display.clear();
    display.drawString(20, 10, "IP: " + WiFi.localIP().toString());
    display.display();

    /*******************************/
    /* Send the data to ThingSpeak */
    /*******************************/
    logdata(1,h);
    logdata(2,t);
    //logdata(3,h);
    //logdata(4,t);
    //logdata(5,t);

    
    digitalWrite(led, HIGH);
    delay(5000);
    digitalWrite(led, LOW);
    delay(5000);

    display.clear();
    display.drawString(20, 10, "Going to Sleep...");
    display.display();
    delay(5000);
    display.clear();
    display.display();

    t = t+1;    // Temporary increment value
    h = h+2;    // Temporary increment value

    /******************************/
    /* Enter into Deep Sleep Mode */
    /******************************/
    //esp_deep_sleep(180000000); //this also works
    esp_deep_sleep_enable_timer_wakeup(30000000);   // 30 seconds
    esp_deep_sleep_start();
}


void loop() {
  // Nothing here...
  // Everything is done at wake up
}

void logdata(int field, float x) 
{
    String poststr="http://api.thingspeak.com/update?api_key=Z6HYA7HOP6UHPMJ4&";
    poststr +="field"+String(field)+"="+String(x);
      Serial.println(poststr);
    http.begin(poststr);
     int  httpCode=http.GET();
          if (httpCode > 0) { //Check for the returning code
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
    if(counter==0 or counter==1)   preferences.putUInt("counter", 1);   //this writes into the ROM
    if(counter==2)   preferences.putUInt("counter", 2);                 //this writes into the ROM
    display.print(" WiFi-");
    display.print(counter); 
    display.print("  ");
    display.println(field); 
    display.display();      
          }
        else {
          Serial.println("Error on HTTP request");
          display.println(" failed...");
          display.display();
    if(counter==0 or counter==1)   preferences.putUInt("counter", 2);
    if(counter==2)   preferences.putUInt("counter", 1);
       preferences.end();
          fflush(stdout);
          esp_restart();     //Restart ESP it does not get connected for long

        }
     

    WIFI_PS_NONE;  //Reduce modem power
    //esp_wifi_stop();
    delay(15000);
    esp_wifi_start();
}

void wifi1() 
{
     WiFi.begin(ssid, pass);
     while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
}

