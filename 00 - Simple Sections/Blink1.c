

// the setup function runs once when you press reset or power the board
int ledPin = 12;
int switchPin = 2;              // switch is connected to pin 2
int var;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  Serial.print("Read switch input: ");
  /* Read the pin and display the value */
  Serial.println(digitalRead(switchPin));

  var = digitalRead(switchPin); // read the pin and save it into var
  
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                   // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                   // wait for a second
}
