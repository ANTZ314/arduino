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

const int led1 =  2;        // the number of the LED pin
const int led2 =  17;       // the number of the LED pin

void setup() {
  // initialize the LEDs pins as an output:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        delay(500);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        delay(500);
}


