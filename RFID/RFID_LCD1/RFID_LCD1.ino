/*
Description:
Combination of the LCD hello world second counter example and the
Standard Addicore_RFID_Example code 

The LCD circuit:
 * LCD RS pin to digital pin 7
 * LCD En pin to digital pin 6
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

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

// include the library code:
#include <LiquidCrystal.h>
#include <AddicoreRFID.h>
#include <SPI.h>

#define uchar unsigned char
#define uint  unsigned int

uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID;                  // create AddicoreRFID object to control the RFID module
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);  // initialize the library with the numbers of the interface pins

/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 5;

//Maximum length of the array
#define MAX_LEN 16

void setup() {
  /*** RFID ***/  
  Serial.begin(9600);                        // RFID reader SOUT pin connected to Serial RX pin at 9600bps 
 
  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
    digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
    digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();  

  /*** LCD ***/  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("RFID LOCKING: ");                 // On the first line
}

/*void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}*/
void loop()
{
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

  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);

  /* Print to LCD */
  if(card == 1){
    lcd.print("   White Card");     // print which card
    card = 0;                       // reset card ID
  }
  else if(card == 2){
    lcd.print("   Blue Tag");     // print which card
    card = 0;                     // reset card ID
  }
  else if(card == 3){
    lcd.print("   Gautrain");     // print which card
    card = 0;                     // reset card ID
  }
  else{
    // Do nothing
  }
}
