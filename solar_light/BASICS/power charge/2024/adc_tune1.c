/*
Simple Analogue Read
*/

const int Relay1 = 3;                           // Battery Relay
const int Relay2 = 4;                           // Solar Panel Relay
const int buttonPin = 11;                       // Pushbutton pin
const int ledPin = 13;                          // BUilt-in LED pin <--- REMOVE
int buttonState = 0;                            // pushbutton status

void setup() {
  pinMode(ledPin, OUTPUT);                      // init Onboard LED
  pinMode(buttonPin, INPUT);                    // init pushbutton
  pinMode(Relay1, OUTPUT);                      // init Relay 1
  pinMode(Relay2, OUTPUT);                      // init Relay 2
}

void loop() {
  
  buttonState = digitalRead(buttonPin);         // read pushbutton value

  /* Button pressed */
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);                 // turn LED on
    delay(500);                                 // Debounce press
    Power_Check();                              // Read Battery voltage
  } else {
    digitalWrite(ledPin, LOW);                  // turn LED off
  }
}

void Power_Check(void){  
  digitalWrite(Relay1, LOW);                    // Open Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                    // Open Relay2
  delay(100);

  digitalWrite(Relay1, HIGH);                   // Close Relay1
  delay(15000);

  digitalWrite(Relay1, LOW);                    // Close Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                    // Open Relay2
  delay(100);

  digitalWrite(Relay2, HIGH);                   // Open Relay2
  delay(15000);

  digitalWrite(Relay1, LOW);                    // Open Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                    // Open Relay2
  delay(100);
  /*________________________________________*/
  /** NOTE: Convert back to real voltages! **/
  /*________________________________________*/
}