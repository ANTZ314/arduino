/*
 Based on Open Source LED Fade Sketch
 Added button debounce and latching functions
 */
int switchPin = 2;     // switch is connected to pin 2
int led1 = 9;          // First PWM pin
int led2 = 10;         // Second PWM pin
int led3 = 11;         // Third PWM pin

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
  /* FADING FUNCTION */
  if(mode == 0){
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
  /* BLINK FUNCTION - REPLACE WITH RFID */
  else if(mode == 1){
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

