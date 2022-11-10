/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "SSD1306.h"

SSD1306  display(0x3c, 21, 22);
int counter = 0;

void setup()   
{
    display.clear();
    display.drawString(0,0, "Counter: " + String(counter));
    display.display();
 
    counter ++;
    delay(1000);
}


void loop() {

}

