/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers
*********************************************************************/
#include <SPI.h>
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
