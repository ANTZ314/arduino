/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers
*********************************************************************/
//#include <SPI.h>
#include <Wire.h>
#include "SSD1306.h"

#define SDA_PIN 21// GPIO21 -> SDA
#define SCL_PIN 22// GPIO22 -> SCL
#define SSD_ADDRESS 0x3c

SSD1306  display(SSD_ADDRESS, SDA_PIN, SCL_PIN);

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
