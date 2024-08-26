/*
 * LED Blinking
 * Code Written by TechMartian
 */
const int MTCK = 13;

 void setup() {
  //pinMode (ledPin, OUTPUT);
  //pinMode (LED_BUILTIN, OUTPUT);
  pinMode (MTCK, OUTPUT);
}

void loop() 
{
  digitalWrite (MTCK, HIGH);    // turn on the LED
  delay(1000);                  // wait 500ms
  digitalWrite (MTCK, LOW);     // turn on the LED
  delay(500);                  // wait 500ms
}