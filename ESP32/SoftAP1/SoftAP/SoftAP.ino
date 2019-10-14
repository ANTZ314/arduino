/*
Programming Instruction:
------------------------
* 5V supply switch on programmer
* PA0 is pulled to ground (jumper)
* Permissions:  $ sudo chmod a+rw /dev/ttyUSB0
* External 5V power on board
* Then remove jumper and restart power
*/

#include <stdio.h>
#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "Pretty_Fly_For_a_WiFi";
const char* password = "#Mexicana411";
//const char* ssid     = "ESP32MY";
//const char* password = "12345678";

WiFiServer server(80);

const int led1 =  2;      // the number of the LED pin
const int led2 =  17;      // the number of the LED pin
float Usb1 = 0.5;
int Usb2 = 1;
int plug1 = 4;
int plug2 = 6;

// Client variables 
char linebuf[80];
int charcount=0;

void setup() {
  // initialize the LEDs pins as an output:
  pinMode(led1, OUTPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while(!Serial) {
    Serial.print("."); // wait for serial port to connect. Needed for native USB port only
  }

  // We start by connecting to a WiFi network
  Serial.println();

  //WiFi.begin(ssid, password);
  //WiFi.softAP(ssid, password);          //Added for soft AP****************

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  //Serial.println(WiFi.softAPIP());      //Added for soft AP****************
  
  server.begin();
}

void loop() 
{
    digitalWrite(led1, 1);
    delay(300);
    digitalWrite(led1, 0);
    delay(300);
    
}


