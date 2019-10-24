/*
 * Test Monichron Vibrator Motor
 * Code Written by A.Smith
 */
const int VBTR = 14;
const int GPIO1 = 13;

 void setup() {
  pinMode (VBTR, OUTPUT);
  pinMode (GPIO1, OUTPUT);
}

void loop() 
{
  digitalWrite (GPIO1, HIGH);    // turn on the LED
  digitalWrite (VBTR, HIGH);    // turn on the LED
  delay(2000);                  // wait 500ms
  digitalWrite (VBTR, LOW);     // turn on the LED
  digitalWrite (GPIO1, LOW);     // turn on the LED
  delay(1000);                  // wait 500ms
}