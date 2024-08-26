/*
 * Book-Case control written for Arduino Micro
 * Based on Open Source LED Fade Sketch
 * Added button debounce and latching functions
 */
int switchPin = 7;     // switch is connected to pin 2
int led1 = 3;          // 1st PWM pin (top shelf)
int led2 = 5;          // 2nd PWM pin (middle shelf)
int led3 = 6;          // 3rd PWM pin (bottom shelf)
int led4 = 11;         // On/Off pin  (hidden compartment)

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

int btn;               // variable for reading the pin status
int btn2;              // variable for reading the delayed status
int buttonState;       // variable to hold the button state
int mode;              // 2 state function modes

// the setup routine runs once when you press reset:
void setup() {
  pinMode(switchPin, INPUT);              // Set the switch pin as input
  pinMode(led1, OUTPUT);                  // declare pin 9 & 10 to be outputs:
  pinMode(led2, OUTPUT);                  // declare pin 9 & 10 to be outputs:
  pinMode(led3, OUTPUT);                  // declare pin 11 to be an output:
  pinMode(led4, OUTPUT);                  // declare pin 11 to be an output:
  buttonState = digitalRead(switchPin);   // read the initial state
}

// the loop routine runs over and over again forever:
void loop() {
  btn = digitalRead(switchPin);       // read input value and store it in val
  delay(25);                          // 10 milliseconds is a good amount of time
  btn2 = digitalRead(switchPin);      // read the input again to check for bounces
  /* BUTTON LATCHING */
  if(btn == btn2){
    if(btn != buttonState){
      if(btn == LOW){
        if(mode == 0){
          mode = 1;                       // Toggle mode state
          delay(500);                     // avoid switching continuously
        }
        else if(mode == 1){
          mode = 0;                       // Toggle mode state
          delay(500);                     // avoid switching continuously
        }
      }
    }
    buttonState = btn;
  }
  /* FADING FUNCTION - NORMAL OPERATION */
  if(mode == 0){
    // Turn off hidden compartment
    digitalWrite(led4, LOW);
    // set the brightness of pin 9:
    analogWrite(led1, brightness);
    analogWrite(led2, brightness);    
    analogWrite(led3, brightness);
  
    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;
  
    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
  /* BLINK FUNCTION - WHEN TOP IS OPEN */
  else if(mode == 1){
    // Turn on hidden compartment
    digitalWrite(led4, HIGH);
    // Rotate blinking each shelf
    digitalWrite(led1, HIGH);
    delay(300);
    digitalWrite(led1, LOW);
    delay(300);
    digitalWrite(led2, HIGH);
    delay(300);
    digitalWrite(led2, LOW);
    delay(300);
    digitalWrite(led3, HIGH);
    delay(300);
    digitalWrite(led3, LOW);
    delay(300);
  }
}

