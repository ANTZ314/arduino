/* 
Test the LED switching relay from LDR Light Sensor
Relay:
  GPIO = D21
  Vcc = 5V
  LEDs = 9V reg from 16v8 direct input
LDR diode: 
  Vcc = 3V3
  ADC pin = A1 (D15)
*/

const int Relay1 = 21;                      // Pin D21
int sensorValue;
float voltage;

/* SETUP */
void setup() {
  pinMode(LED_BUILTIN , OUTPUT);
  pinMode(Relay1, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  sensorValue = analogRead(A1);             // Analog pin 1
  voltage = sensorValue * (3.3 / 1023.0);   // analog (0 - 1023) to voltage (0 - 3V3)

  /* Range: Bright(2.8V) - Dim(1.3V) */
  if(voltage <= 1.8){
    digitalWrite(LED_BUILTIN, HIGH);        // turn the LED on
    digitalWrite(Relay1, HIGH);               // Close Relay1
    delay(200);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);         // turn the LED off
    digitalWrite(Relay1, LOW);                // Open Relay1
    delay(200);
  }

}