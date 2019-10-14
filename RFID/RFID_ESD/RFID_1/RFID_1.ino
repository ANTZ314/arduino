/*
 Example sketch to read the ID from an Addicore 13.56MHz RFID tag
 as found in the RFID AddiKit found at: 
 http://www.addicore.com/RFID-AddiKit-with-RC522-MIFARE-Module-RFID-Cards-p/126.htm

  
*/

#include <AddicoreRFID.h>
#include <SPI.h>

#define  uchar unsigned char
#define uint  unsigned int

uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module

/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
//const int chipSelectPin = 7;              // was 10 (reset?)
//const int NRSTPD        = 5;
const int lockLED       = 2;                // LED Pin
const int unlockLED     = 4;                // LED Pin
const int button        = A1;                // 
//const char i;

//Maximum length of the array
#define MAX_LEN 16

int buttonState = 0;                          // variable for reading the pushbutton status

void setup() {                
   Serial.begin(9600);                        // RFID reader SOUT pin connected to Serial RX pin at 9600bps 
 
  // start the SPI library:
  SPI.begin();

  //pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
  //  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  //pinMode(NRSTPD,OUTPUT);                     // Not Reset and Power-down
  // digitalWrite(NRSTPD, HIGH);
    
  pinMode(button, INPUT);
  pinMode(lockLED, OUTPUT);                   // set as output
    digitalWrite(lockLED, LOW);               // start off
  pinMode(unlockLED, OUTPUT);                   // set as output
    digitalWrite(unlockLED, LOW);               // start off
    
  myRFID.AddicoreRFID_Init();  
}

void blink_led(){
    for(int i=0;i<3;i++)
    {
      digitalWrite(lockLED, LOW);
      delay(500);
      digitalWrite(lockLED, HIGH);
      delay(500);
    }
}

void locked(){
  digitalWrite(lockLED, HIGH);
  delay(500);
  digitalWrite(unlockLED, LOW);
}

void unlocked(){
  digitalWrite(unlockLED, HIGH);
  delay(500);
  digitalWrite(lockLED, LOW);
}

void loop()
{
  uchar i, tmp, checksum1;
  uchar status;
        uchar str[MAX_LEN];
        uchar RC_size;
        uchar blockAddr;  //Selection operation block address 0 to 63
        String mynum = "";

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
 else{
      //Serial.println("NOPE");
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

            buttonState = digitalRead(button);
            
            // Should really check all pairs, but for now we'll just use the first
            if(str[0] == 197)                           //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
            {
              if(buttonState == LOW)
              {
                Serial.println("\nUNLOCKED!!\n");
                unlocked();
              }
              else{
                Serial.println("\nSTILL LOCKED!!\n");
                blink_led();
              }
            } 
            else if(str[0] == 245) {                    //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
                Serial.println("\nLOCKED!!\n");
                locked();              // Blink LED
                
            }
            Serial.println();
            delay(1000);
  }
    
        myRFID.AddicoreRFID_Halt();      //Command tag into hibernation              

}


