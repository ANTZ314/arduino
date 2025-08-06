/* 
Test and configure light sensitivity of the LDR diode 
Vcc = 3V3
ADC pin = A1 (D15)
*/

/* SETUP */
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN , OUTPUT);
}

void loop() {
  
  int sensorValue = analogRead(A1);             // Analog pin 1
  float voltage = sensorValue * (3.3 / 1023.0); // analog (0 - 1023) to voltage (0 - 3V3)
  Serial.println(voltage);                      // print out the value you read

  if(voltage >= 512){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  }
}