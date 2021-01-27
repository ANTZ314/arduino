/*
 *  Counting presses
 */

int switchPin = 2;              // switch is connected to pin 2
int val;                        // variable for reading the pin status
int buttonState;                // variable to hold the button state
int buttonPresses = 0;          // how many times the button has been pressed

void setup() {
  pinMode(switchPin, INPUT);    // Set the switch pin as input

  Serial.begin(9600);           // Set up serial communication at 9600bps
  buttonState = digitalRead(switchPin);   // read the initial state
}


void loop(){
  val = digitalRead(switchPin);      // read input value and store it in val

  if (val != buttonState) {          // the button state has changed!
    if (val == LOW) {                // check if the button is pressed
      buttonPresses++;               // increment the buttonPresses variable
      Serial.print("Button has been pressed ");
      Serial.print(buttonPresses);
      Serial.println(" times");
    }
  }
  buttonState = val;                 // save the new state in our variable
}