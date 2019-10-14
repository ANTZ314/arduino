/***
 * SPI-MRF522 RFID card reader with OLED display for ESP32
 * Code put together by A.SMITH
 * 
 * Arduino Settings:
 * Board: ESP32 Dev Module
 * Upload Speed: 921600 
 * Port: ttyUSB0
 * Flash Mode QIO
 * 
 * RFID DEFAULT SPI PINS:
 * 18 - SCK
 * 19 - MISO
 * 23 - MOSI
 *  4 - RST
 *  5 - CS (SDA)
 * 
 * OLED DEFAULT I2C PINS:
 * 21 - SDA
 * 22 - SCL
 */
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include "SSD1306.h" 

#define SDA 21
#define SCL 22

const int resetPin = 4;       // Reset pin
const int ssPin = 5;          // Slave select pin
const int GREEN = 25;
const int RED = 26;
const int BLUE = 27;
const int LOCK = 33;
//const int PushButton = 34;
//const int LED_BUILTIN = 2;  // Onboard LED
int tag = 0;                  // RFID trigger flag
int access = 0;               // success/denied flag

// Redefine these pins
MFRC522 mfrc522 = MFRC522(ssPin, resetPin); // Create instance

// Initialise the OLED
SSD1306  display(0x3c, SDA, SCL);

 
void setup() {
  //pinMode (LED_BUILTIN, OUTPUT);
  //pinMode(PushButton, INPUT);
  pinMode (RED, OUTPUT);
  pinMode (BLUE, OUTPUT);
  pinMode (GREEN, OUTPUT);
  pinMode (LOCK, OUTPUT);
  
  Serial.begin(115200);
  SPI.begin();   
 
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial(); 
 
  display.init();
  digitalWrite (LOCK, HIGH);     // intially LOCKED
}
 
void loop() {
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
    }
     
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
    }
     
    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(mfrc522.uid.uidByte[i], HEX);
       content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    
    // UID of the cards given access
    if (content.substring(1) == "F5 F9 AE 75") 
    {
        tag = 1;                        // tag attempt
        access = 1;                     // successful tag
        Serial.println("ACCESS GRANTED!!");
        Serial.println();
  
        display.clear();
        //display.setTextSize(2);
        display.drawString(10, 0, "ACCESS GRANTED!!");
        display.display();    
        delay(3000);
        display.clear();
        display.drawString(20, 10, "PRESENT TAG...");
        display.display();
    }
   
    else   
    {
         tag = 1;                        // tag attempt
         access = 0;                     // successful tag
         Serial.println(" ACCESS DENIED!!");
         Serial.println();
   
         display.clear();
         display.drawString(10, 0, "ACCESS DENIED");
         display.display();
         delay(3000);
         display.clear();
         display.drawString(20, 10, "PRESENT TAG...");
         display.display();
   }
   if (tag == 1)
   {
       if(access == 1)
       {
           access = 0;                    // reset flag
           /* Unlock the door */
           digitalWrite(GREEN, HIGH); 
           digitalWrite (LOCK, LOW);      // LOCK ON (OPEN)
           delay(4000);
           digitalWrite (LOCK, HIGH);     // LOCK OFF (LOCKED)
           digitalWrite (GREEN, LOW);     // turn off the LED

           Serial.println("UNLOCK DOOR!!");
           Serial.println();
        }
        else
        {
           /* Denied Indicator */
           digitalWrite (RED, HIGH);      // turn on the LED
           delay(1000);                    // wait for half a second or 500 milliseconds
           digitalWrite (RED, LOW);       // turn off the LED
           delay(1000);                    // wait for half a second or 500 milliseconds 
           
           Serial.println("DOOR STILL LOCKED!!");
           Serial.println();
        }
       tag = 0;                           // reset flag
    }
        Serial.println(".");
        /* Status Indicator */
        digitalWrite(BLUE, HIGH); 
        delay(500);                       // wait for half a second or 500 milliseconds
        digitalWrite (BLUE, LOW);         // turn off the LED
        delay(500);                       // wait for half a second or 500 milliseconds 
  
  }
