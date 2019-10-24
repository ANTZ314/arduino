/*
 * LED Blinking
 * Code Written by A.Smith
 */
const int GPIO1 = 13;

 void setup() {
  pinMode (GPIO1, OUTPUT);
}

void loop() 
{
  digitalWrite (GPIO1, HIGH);    // turn on the LED
  delay(1000);                  // wait 500ms
  digitalWrite (GPIO1, LOW);     // turn on the LED
  delay(500);                  // wait 500ms
}