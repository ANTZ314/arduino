/***
 * LED Blinking
 * Code Written by TechMartian
 */
const int GREEN = 25;
const int RED = 26;
const int BLUE = 27;
const int PushButton = 34;
 
void setup() {
  pinMode (RED, OUTPUT);
  pinMode (BLUE, OUTPUT);
  pinMode (GREEN, OUTPUT);
  pinMode(PushButton, INPUT);

}

void loop() {
  int Push_button_state = digitalRead(PushButton);
  
  if ( Push_button_state == HIGH )
  { 
    digitalWrite (RED, HIGH);  // turn on the LED
    delay(500); // wait for half a second or 500 milliseconds
    digitalWrite (RED, LOW);   // turn off the LED
    delay(500); // wait for half a second or 500 milliseconds 
  }
  else 
  {
    digitalWrite(BLUE, HIGH); 
    delay(500); // wait for half a second or 500 milliseconds
    digitalWrite (BLUE, LOW);   // turn off the LED
    delay(500); // wait for half a second or 500 milliseconds 
    digitalWrite(GREEN, HIGH); 
    delay(500); // wait for half a second or 500 milliseconds
    digitalWrite (GREEN, LOW);   // turn off the LED
    delay(500); // wait for half a second or 500 milliseconds
  }
}
