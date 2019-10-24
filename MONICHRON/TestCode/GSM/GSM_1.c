/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
 * 
*/
#include <HardwareSerial.h>
#define RXD2 16
#define TXD2 17

String AT;
String response = "OK";            // output from string functions placed here

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);          // Serial port 2
  
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  delay(500);
}
 
void loop() {
  /* If Tx looped to Rx - print what is sent */
  //while (Serial2.available()){ Serial.print(char(Serial2.read())); }

  Serial2.println("AT");
  
  /* Get response */
  AT = Serial2.readString();
  /* Check if correct response */
  if (AT == response){
    Serial.println("AT response -> OKAY!");
  }
  else{
    Serial.println("AT response -> FAILED!");
  }
  
}



