/*
Simple Voltage Check (to tune varistors for 5V ADC input):
 -> Button press closes one Relay and opens the other (15 sec)
 -> 2x Battery Relay then toggles to 2x Solar Relay (Repeat)
 -> Includes Charge Relay to see if charge current flowing - Working
*/

const int Relay1 = 3;                           // Battery Relay      D3
const int Relay2 = 4;                           // Solar Panel Relay  D4
const int Relay3 = 9;                           // Charge Relay       D9
const int buttonPin = 11;                       // Pushbutton pin     D11
const int ledPin = 13;                          // BUilt-in LED pin <--- REMOVE
int buttonState = 0;                            // pushbutton status
int cntr = 0;
int toggle = 0;                                 // toggle between 3 relays

void setup() {
  //Serial.begin(9600);
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
    Power_Check(toggle);                        // Read Battery voltage

    cntr++;
    if(cntr == 2){                              // 2 checks each
      toggle++;                                 // switch power source
      cntr = 0;

      if(toggle == 3){toggle = 0;}              // back to Relay 1
    }

  } else {
    digitalWrite(ledPin, LOW);                  // turn LED off
  }
}

void Power_Check(int alt){  
  /* Open All */
  digitalWrite(Relay1, LOW);                    // Open Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                    // Open Relay2
  delay(100);
  digitalWrite(Relay3, LOW);                    // Open Relay2
  delay(100);
  /* Close One */
  if(alt == 0){
    digitalWrite(Relay1, HIGH);                 // Close Relay1 (Battery)
    delay(15000);
  }
  else if(alt == 1){
    digitalWrite(Relay2, HIGH);                 // Close Relay2 (Solar)
    delay(15000);
  }
  else if(alt == 2){
    digitalWrite(Relay3, HIGH);                 // Close Relay3 (Charge)
    delay(15000);
  }
  /* Open All */
  digitalWrite(Relay1, LOW);                    // Open Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                    // Open Relay2
  delay(100);
  digitalWrite(Relay3, LOW);                    // Open Relay2
  delay(100);
}