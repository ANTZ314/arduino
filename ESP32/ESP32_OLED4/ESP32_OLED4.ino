/***
 * OLED display for ESP32
 * Code Written by A.SMITH
 * 
 * Arduino Settings:
 * Board: ESP32 Dev Module
 * 
 * DEFAULT I2C PINS:
 * 21 - SDA
 * 22 - SCL
 */

// Include the correct display library
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"

// Initialize the OLED display using i2c
#define SDA 21
#define SCL 22

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, SDA, SCL);

#define button 4
#define led 2
#define DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;
int BTN = 0;
String ID = "525234899";
int modeSet = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  pinMode(button,INPUT);
  pinMode(led,OUTPUT);

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}

void drawFontFace() {
    /* create more fonts at http://oleddisplay.squix.ch/  */

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(18, 20, "--DATA--");
    display.setFont(ArialMT_Plain_10);
    display.drawString(15, 50, "User ID: " + ID);
}

void drawProgressBar() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}


Demo demos[] = {drawFontFace, drawProgressBar};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void loop() {
  // clear the display
  display.clear();
  // draw the current method
  demos[demoMode]();
  
  // write the buffer to the display
  display.display();

  //Serial.println(".");
  BTN = digitalRead(button);
  
  if(BTN == LOW){
      // Toggle
      if(modeSet == 1){
          modeSet = 0;
      }
      else{
          modeSet = 1;
      }
      //modeSet != modeSet;
      delay(50);
  }

  if(modeSet == true){
    demoMode = 0;
    digitalWrite(led, LOW);
    //delay(500);
    //Serial.println("0");
  }
  else{
    demoMode = 1;
    digitalWrite(led, HIGH);
    //delay(500);
    //Serial.println("1");
  }
  
  /*if (millis() - timeSinceLastModeSwitch > DURATION) {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  } */
  counter++;
  delay(25);
}
