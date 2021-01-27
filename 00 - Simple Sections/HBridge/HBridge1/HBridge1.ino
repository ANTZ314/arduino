/* H-Bridge */

int switchPin = 2;              // switch is connected to pin 2
int HB1Pin = 12;
int HB2Pin = 11;

int val;                        // variable for reading the pin status
int val2;                       // variable for reading the delayed/debounced status
int buttonState;                // variable to hold the button state

int flag1 = 0;                          // Set outputs once
int flag2 = 0;                          // Set outputs once

void setup() {
  pinMode(switchPin, INPUT);            // Set the switch pin as input
  pinMode(HB1Pin, OUTPUT);
  pinMode(HB2Pin, OUTPUT);
  
  //Serial.begin(9600);                 // Set up serial communication at 9600bps
  buttonState = digitalRead(switchPin); // read the initial state
}

void loop(){
  val = digitalRead(switchPin);         // read input value and store it in val
  delay(50);                            // 50 milliseconds debounce
  val2 = digitalRead(switchPin);        // read the input again to check for bounces
  //if (val == val2) {                  // make sure we got 2 consistant readings!
    //if (val != buttonState) {         // the button state has changed!
      if (val == LOW) {                 // check if the button is pressed
        if(flag1 == 0){                 // Do once
          flag1 = 1;                    // do once
          flag2 = 0;                    // setup for next state
          digitalWrite(HB1Pin, LOW);
          digitalWrite(HB2Pin, LOW);
          delay(100);
          digitalWrite(HB1Pin, HIGH);
          digitalWrite(HB2Pin, LOW);
        }
      } else {
        if(flag2 == 0){                 // do once
          flag2 = 1;                    // do once
          flag1 = 0;                    // setup for next state
          digitalWrite(HB1Pin, LOW);
          digitalWrite(HB2Pin, LOW);
          delay(100);
          digitalWrite(HB1Pin, LOW);
          digitalWrite(HB2Pin, HIGH);
        }
      }
    //}
    buttonState = val;                 // save the new state in our variable
  //}
}

