/* 
 *  OLED [32x64]??
 *  ESP32 - I2C - OLED-SSD1306
 *  SDA - D21 - D0 (pin23)
 *  SCL - D22 - D4 (pin24)
 *  
 *  display.setFont(ArialMT_Plain_10);  <- 10, 16, 24
 *  display.drawString(x, y1, "STRING1");
 *  display.drawString(x, y2, "STRING2");
 *  display.display();
 */
#include <Wire.h>
#include "SSD1306.h" 

const int MTCK = 13;
//SSD1306  display(0x3c, 21, 22); // ESP32-Dev-Kit
SSD1306  display(0x3c, 0, 4);     // Monichron Board
 
void setup() {
  pinMode (MTCK, OUTPUT);                   // GPIO LED
  display.init();
  display.setFont(ArialMT_Plain_10);
  display.drawString(10, 5, "HEART RATE:");
  display.setFont(ArialMT_Plain_16);
  display.drawString(20, 20, "86 BPM");
  display.display();
}
 
void loop() 
{
  digitalWrite (MTCK, HIGH);    // turn on the LED
  delay(1000);                  // wait 500ms
  digitalWrite (MTCK, LOW);     // turn on the LED
  delay(500);                   // wait 500ms
}