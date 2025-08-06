/* 
Test the two voltage switching relays

NOTE: Nano-Every: Tools -> Old Boot Loader Option
*/

const int Relay1 = 3;                // Pin D8
const int Relay2 = 4;                // Pin D8

/* SETUP */
void setup() {
  pinMode(LED_BUILTIN , OUTPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
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
}
