/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x32 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B00000000,
  B00000000, B00000000,
  B00000011, B11000000,
  B00000110, B01100000,
  B00000110, B01100000,
  B00000110, B01100000,
  B00000110, B01100000,
  B00000110, B01100000,
  B00000110, B01100000,
  B00000110, B01100000,
  B00000110, B01100000,
  B00000110, B01101000,
  B00110111, B00110111,
  B01100011, B01100011,
  B01100110, B01100110,
  B00011000, B00011000 };

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {                
  Serial.begin(9600);

  // initialize with the I2C addr 0x3C (for the 128x32)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // intialized with an Adafruit splashscreen
  display.display();
  delay(2000);
  display.clearDisplay();

  // draw scrolling text
  scrolltext1();
  delay(4000);
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("YOUR MOM'S VAGINA");
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  //display.println(3.1415926535);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("IS LARGE");
  //display.print("0x"); display.println(0xFEEDBADD, HEX);
  display.display();
  delay(5000);
  display.clearDisplay();

  // draw scrolling text
  scrolltext2();
  delay(4000);
  display.clearDisplay();

  // miniature bitmap display
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();
  delay(1);

  // invert the display
  display.invertDisplay(true);
  delay(2000); 
  display.invertDisplay(false);
  delay(1000); 
  display.clearDisplay();

    // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}


void loop() {
  
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
   }
}

void scrolltext1(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("LOCKED");
  display.display();
  delay(1);
 
  //display.startscrollright(0x00, 0x0F);
  //delay(2000);
  //display.stopscroll();
  //delay(1000);
  //display.startscrollleft(0x00, 0x0F);
  //delay(2000);
  //display.stopscroll();
  //delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(3000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(3000);
  display.stopscroll();
}

void scrolltext2(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("UNLOCKED");
  display.display();
  delay(1);
   
  display.startscrolldiagright(0x00, 0x07);
  delay(3000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(3000);
  display.stopscroll();
}
