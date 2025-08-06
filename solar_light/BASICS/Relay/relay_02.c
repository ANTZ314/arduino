/* Tools -> Old Boot Loader Option */

const int Relay1 = 3;                           // Battery Relay (ADC)
const int Relay2 = 9;                           // Solar Panel Relay (ADC)
const int Relay3 = 4;                           // Solar Relay (Charge)
const int Relay4 = 21;                          // LED Strip

/* SETUP */
void setup() {
  pinMode(LED_BUILTIN , OUTPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);                      // init Relay 3
  pinMode(Relay4, OUTPUT);                      // init Relay 4
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on
  digitalWrite(Relay1, HIGH);       // Close Relay1
  delay(3000);                      // wait for 3 seconds
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  digitalWrite(Relay1, LOW);        // Open Relay1
  delay(2000);                      // wait for 2 seconds
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on
  digitalWrite(Relay2, HIGH);       // Close Relay2
  delay(3000);                      // wait for 3 seconds
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  digitalWrite(Relay2, LOW);        // Open Relay
  delay(2000);                      // wait for 2 seconds
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on
  digitalWrite(Relay3, HIGH);       // Close Relay1
  delay(3000);                      // wait for 3 seconds
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  digitalWrite(Relay3, LOW);        // Open Relay1
  delay(2000);                      // wait for 2 seconds
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on
  digitalWrite(Relay4, HIGH);       // Close Relay2
  delay(3000);                      // wait for 3 seconds
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  digitalWrite(Relay4, LOW);        // Open Relay
  delay(2000);                      // wait for 2 seconds
}
