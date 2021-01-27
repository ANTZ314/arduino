/*
 * Based on Open Source LED Fade Sketch
 * Added button debounce and latching functions
 * Remember a pull up on the switch
 */
int switchPin = 2;     // switch is connected to pin 2
int led1 = 9;          // First PWM pin
int led2 = 10;         // Second PWM pin

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

int btn;               // variable for reading the pin status
int btn2;              // variable for reading the delayed status
int buttonState;       // variable to hold the button state
int mode;              // 2 state function modes

// the setup routine runs once when you press reset:
void setup() {
  pinMode(switchPin, INPUT);              // Set the switch pin as input
  pinMode(led1, OUTPUT);                  // declare pin 9 & 10 to be an outputs:
  pinMode(led2, OUTPUT);                  // declare pin 9 & 10 to be an outputs:
  buttonState = digitalRead(switchPin);   // read the initial state
}

// the loop routine runs over and over again forever:
void loop() {
  btn = digitalRead(switchPin);       // read input value and store it in val
  delay(25);                          // 10 milliseconds is a good amount of time
  btn2 = digitalRead(switchPin);      // read the input again to check for bounces
  
  if(btn == btn2){
    if(btn != buttonState){
      if(btn == LOW){
        if(mode == 0){
          mode = 1;                       // Toggle mode state
          digitalWrite(led1, HIGH);
          delay(500);                     // avoid switching continuously
        }
        else if(mode == 1){
          mode = 0;                       // Toggle mode state
          digitalWrite(led1, LOW);
          delay(500);                     // avoid switching continuously
        }
      }
    }
    buttonState = btn;
  }
}
