

const int ledPin = 13;          // Output: BUilt-in LED
const int PIR_SENSOR = 6;       // PIR Sensor Pin
int pinStateCurrent   = LOW;    // current state of pin
int pinStatePrevious  = LOW;    // previous state of pin

void setup() {
  Serial.begin(9600);           // initialize serial
  pinMode(PIR_SENSOR, INPUT);   // PIR Input
  pinMode(ledPin, OUTPUT);      // PIR Input
}

void loop() {
  pinStatePrevious = pinStateCurrent;                         // store old state
  pinStateCurrent = digitalRead(PIR_SENSOR);                  // read new state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    digitalWrite(ledPin, HIGH);                               // turn on-board LED on
  }
  else
  if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    digitalWrite(ledPin, LOW);                                // turn on-board LED off    
  }
}