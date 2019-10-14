
/* constants won't change. They're used here to set pin numbers: */
const int button = A1;     // the number of the pushbutton pin
const int ledPin1 =  A0;      // the number of the LED pin
const int ledPin2 =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(button, INPUT);
  pinMode(ledPin1,OUTPUT);                 // set the pin to input
  pinMode(ledPin2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  buttonState = digitalRead(button);
  
  if (buttonState == HIGH){                // it shows pressed
    digitalWrite(ledPin1, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(ledPin2, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else if (buttonState == LOW){            // it shows not pressed
    digitalWrite(ledPin1, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(ledPin2, LOW);    // turn the LED off by making the voltage LOW
  }
  /*
  digitalWrite(ledPin1, HIGH);      // turn the LED on (HIGH is the voltage level)
  digitalWrite(ledPin2, HIGH);      // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(ledPin1, LOW);       // turn the LED off by making the voltage LOW
  digitalWrite(ledPin2, LOW);       // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
  */
}