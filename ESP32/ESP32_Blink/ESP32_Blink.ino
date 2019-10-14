/***
 * LED Blinking
 * Code Written by TechMartian
 */
const int ledPin = 5;
const int LED_BUILTIN = 2;

 void setup() {
  pinMode (ledPin, OUTPUT);
  pinMode (LED_BUILTIN, OUTPUT);
  
}

void loop() 
{
  digitalWrite (LED_BUILTIN, HIGH);  // turn on the LED
  digitalWrite (ledPin, HIGH);  // turn on the LED
  delay(500);  // wait for half a second or 500 milliseconds
  digitalWrite (LED_BUILTIN, LOW);  // turn off the LED
  digitalWrite (ledPin, LOW);  // turn off the LED
  delay(100);  // wait for half a second or 500 milliseconds
}
