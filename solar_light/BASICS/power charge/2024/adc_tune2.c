/*
Simple Voltage Check (to tune varistors for 5V ADC input):
 -> Button press closes one Relay and opens the other (15 sec)
 -> 3x Battery Relay then toggles to 3x Solar Relay (Repeat)
*/

const int Relay1 = 3;                           // Battery Relay D3
const int Relay2 = 4;                           // Solar Panel Relay D4
const int buttonPin = 11;                       // Pushbutton pin
const int ledPin = 13;                          // BUilt-in LED pin <--- REMOVE
int buttonState = 0;                            // pushbutton status
int nwline = 0;
boolean toggle = false;

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

    //Serial.print(voltage);                    // *Voltage undefined*
    //Serial.print("V - ");
    nwline++;
    if(nwline == 3){                            // 2 checks each
      toggle = !toggle;                         // switch power source
      //Serial.println("*");
      nwline = 0;
    }

  } else {
    digitalWrite(ledPin, LOW);                  // turn LED off
  }
}

void Power_Check(boolean alt){  
  digitalWrite(Relay1, LOW);                    // Open Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                    // Open Relay2
  delay(100);

  if(alt == false){
    digitalWrite(Relay1, HIGH);                 // Close Relay1 (Battery)
    delay(15000);
  }
  else if(alt == true){
    digitalWrite(Relay2, HIGH);                 // Open Relay2 (Solar)
    delay(15000);
  }
  digitalWrite(Relay1, LOW);                    // Close Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                    // Open Relay2
  delay(100);

  /*________________________________________*/
  /** NOTE: Convert back to real voltages! **/
  /*________________________________________*/
}