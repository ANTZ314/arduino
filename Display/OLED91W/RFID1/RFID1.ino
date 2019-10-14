/*
Description:
UNO: Combination of the LCD hello world second counter example and the
Standard Addicore_RFID_Example code 

The LCD circuit:
 * OLED SDA pin to digital pin A4
 * OLED SCL pin to digital pin A5
 * OLED GND pin to digital pin GND
 * OLED VCC pin to digital pin 3V3

The RFID circuit:
 * RFID 3V3  pin to digital pin 3V3
 * RFID RST  pin to digital pin 9
 * RFID GND  pin to digital pin GND
 * RFID IRQ  pin to digital pin N/C
 * RFID MISO pin to digital pin 12
 * RFID MOSI pin to digital pin 11
 * RFID CLK  pin to digital pin 13
 * RFID SDA  pin to digital pin 10 
 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AddicoreRFID.h>

#define uchar unsigned char
#define uint  unsigned int

uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID;                  // create AddicoreRFID object to control the RFID module

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

const int chipSelectPin = 10;
const int NRSTPD = 5;

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
 
  /**** SETUP THE RFID ****/

  // start the SPI library:
  SPI.begin();

  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
    digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
    digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();  

  /**** SETUP THE OLED ****/

  // initialize with the I2C addr 0x3C (for the 128x32)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // intialized with an Adafruit splashscreen
  display.display();
  delay(2000);
  display.clearDisplay();

  // MAIN SCREEN
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,0);
  display.println("ACCESS");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15,16);
  display.println("CONTROL");
  display.display(); 
}


void loop() {
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;  //Selection operation block address 0 to 63
  String mynum = "";
  uchar card = 0;

  str[1] = 0x4400;
  //Find tags, return tag type
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str); 
  if (status == MI_OK)
  {
          Serial.println("RFID tag detected");
          Serial.print("Tag Type:\t\t");
          uint tagType = str[0] << 8;
          tagType = tagType + str[1];
          switch (tagType) {
            case 0x4400:
              Serial.println("Mifare UltraLight");
              break;
            case 0x400:
              Serial.println("Mifare One (S50)");
              break;
            case 0x200:
              Serial.println("Mifare One (S70)");
              break;
            case 0x800:
              Serial.println("Mifare Pro (X)");
              break;
            case 0x4403:
              Serial.println("Mifare DESFire");
              break;
            default:
              Serial.println("Unknown");
              break;
          }
  }

  //Anti-collision, return tag serial number 4 bytes
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
          checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
          Serial.print("The tag's number is:\t");
          Serial.print(str[0]);
            Serial.print(" , ");
          Serial.print(str[1]);
            Serial.print(" , ");
          Serial.print(str[2]);
            Serial.print(" , ");
          Serial.println(str[3]);

          Serial.print("Read Checksum:\t\t");
            Serial.println(str[4]);
          Serial.print("Calculated Checksum:\t");
            Serial.println(checksum1);
            
            // MAKE SURE TO CHECK ALL PAIRS
            if(str[0] == 197)                      // White Card
            {
                Serial.println("\nWhite Card!\n");
                card = 1;
            } else if(str[0] == 245) {             // Blue Tag
                Serial.println("\nBlue Tag!!\n");
                card = 2;
            }
            else if(str[0] == 126) {                // Gautrain Card
                Serial.println("\nGautrain Card!!\n");
                card = 3;
            }
            Serial.println();
            delay(1000);
  }
   
  myRFID.AddicoreRFID_Halt();      //Command tag into hibernation

  /* Print to LCD */
  if(card == 1){
    display.clearDisplay();
    // text display tests
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("ACCESS CONTROL");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("WHITE CARD");
    display.display(); 

    delay(5000);
    display.clearDisplay();

    // MAIN SCREEN
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20,0);
    display.println("ACCESS");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(15,16);
    display.println("CONTROL");
    display.display(); 

    card = 0;                       // reset card ID
  }
  else if(card == 2){
    display.clearDisplay();
    // text display tests
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("ACCESS CONTROL");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("BLUE TAG");
    display.display(); 

    delay(5000);
    display.clearDisplay();

    // MAIN SCREEN
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20,0);
    display.println("ACCESS");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(15,16);
    display.println("CONTROL");
    display.display(); 

    card = 0;                     // reset card ID
  }
  else if(card == 3){
    display.clearDisplay();
    // text display tests
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("ACCESS CONTROL");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("GAUTRAIN");
    display.display(); 
    
    delay(5000);
    display.clearDisplay();

    // MAIN SCREEN
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20,0);
    display.println("ACCESS");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(15,16);
    display.println("CONTROL");
    display.display();  

    card = 0;                     // reset card ID
  }
  else{
    // Do Nothing
  }
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
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(500);
  //display.startscrollleft(0x00, 0x0F);
  //delay(2000);
  //display.stopscroll();
  //delay(1000);    
  //display.startscrolldiagright(0x00, 0x07);
  //delay(3000);
  //display.startscrolldiagleft(0x00, 0x07);
  //delay(3000);
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
