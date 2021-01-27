

// the setup function runs once when you press reset or power the board
int ledPin1 = 12;
int ledPin2 = 11;
int switchPin = 2;              // switch is connected to pin 2

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(switchPin, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  //Serial.print("Read switch input: ");
  /* Read the pin and display the value */
  //Serial.println(digitalRead(switchPin));

  //var = digitalRead(switchPin); // read the pin and save it into var
  
  digitalWrite(ledPin1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(ledPin2, LOW);    // turn the LED on (HIGH is the voltage level)
  delay(500);                    // wait for a second
  digitalWrite(ledPin1, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(ledPin2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                   // wait for a second
}
