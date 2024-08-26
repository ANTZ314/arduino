/***
 * SPI-MRF522 RFID card reader
 * Code Written by A.SMITH
 * 
 * DEFAULT SPI PINS:
 * 18 - SCK
 * 19 - MISO
 * 23 - MOSI
 *  4 - RST
 *  5 - CS (SDA)
 */ 
#include <SPI.h>
#include <MFRC522.h>
 
//const int resetPin = 22;    // Reset pin
//const int ssPin = 21;       // Slave select pin
const int resetPin = 4;       // Reset pin
const int ssPin = 5;          // Slave select pin
const int LED_BUILTIN = 2;    // Onboard LED

// Redefine these pins
MFRC522 mfrc522 = MFRC522(ssPin, resetPin); // Create instance
 
void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  SPI.begin();   
 
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();  
 
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
     
    // Show UID on serial monitor
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
    if (content.substring(1) == "E2 CF 57 2F") 
    {
      digitalWrite (LED_BUILTIN, HIGH);   // turn on the LED
      delay(500);                         // wait
      digitalWrite (LED_BUILTIN, LOW);    // turn off the LED
      
      Serial.println("Authorized access");
      Serial.println();
      delay(3000);
    }
   
   else   {
      Serial.println(" Access denied");
      Serial.println();
      delay(3000);
  }
}
