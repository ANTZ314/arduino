/**************************
 * One timer in day and one in night mode
 * PIR motion triggered
 * Timer1 counts down before de-trigger
 * 
 * ************************/

/* Global variables */
const int ledPin = 13;                          // Output: BUilt-in LED <--- REMOVE
const int PIR_SENSOR = 6;                       // Input: PIR digital IO
unsigned long previousMillis = 0;               // Stores the last time
const long interval = 5000;                     // 1 sec Interval
unsigned long previousMillis2 = 0;              // Stores the last time
const long interval2 = 5000;                    // 2-sec Interval
int pinStateCurrent   = LOW;                    // current state of pin
int pinStatePrevious  = LOW;                    // previous state of pin
bool is_day;                                    // Day or Night mode
bool bat_stat;                                  // Battery (bad <= 15.00V)
bool sol_stat;                                  // Solar (bad <= 16.79V)
bool led_bat;                                   // LEDs need (>15.00V)
bool Asnooze = false;                           // Light snooze

void setup() {
  Serial.begin(9600);                           // initialize serial
  pinMode(ledPin, OUTPUT);                      // init Onboard LED
  pinMode(PIR_SENSOR, INPUT);                   // init PIR Sensor
}

/** MAIN LOOP **/
void loop() {

  unsigned long currentMillis = millis();       // Get the current time

  /** User Btn: Display Prev Voltages **/
  //user_check();                               // Check button & display voltages

  /* Every 60sec: Check Day/Night mode */
  day_night();                                  // Is it day or night?

  /** ~~~~~~~~~~NIGHT~~~~~~~~~~~ **/
  if(is_day == false){                          // Range: Bright(2.8V) - Dim(1.3V)
    pinStatePrevious = pinStateCurrent;         // store old state
    pinStateCurrent = digitalRead(PIR_SENSOR);  // read new state

    if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
      Serial.println("Motion detected!");
      /* Night && Battery == Good */
      if(led_bat == true){ Serial.println("Lights ON!"); }      // Lights On
      /* Night && Battery == Bad    */
      else{ Serial.println("Bad Battery!"); }                   // Lights Off
      digitalWrite(ledPin, HIGH);                               // turn on-board LED on
    }
    else
    if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
      Serial.println("Motion stopped!");
      Asnooze = true;
    }
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if(Asnooze == true){
          Asnooze = false;
          Serial.println("Lights OFF!");
          digitalWrite(ledPin, LOW);
        }
      }
  }
  /** ~~~~~~~~~~~DAY~~~~~~~~~~~~ **/
  else if(is_day == true){
    Aziz_Light(false);                            // Lights Off
    
    if (currentMillis - previousMillis2 >= interval2) {
      previousMillis2 = currentMillis;            // Update the last print time
      /* Every 60sec: Check Battery & Solar */
      Power_Check();                              // Battery and Solar Voltages
      Charge_Bat();                               // Decide whether to charge
    }
  }
}

/** Read LDR light sensitiviy **/
void day_night(void) {
  is_day = false;               // Test Night
  //is_day = true;              // Test Day
}

/** Check Battery & Solar Voltages [550 millisec] **/
void Power_Check(void){
  delay(150);
  Serial.println("All Relays OFF!");
  delay(200);
  Serial.println("Check Battery!");
  delay(200);
  Serial.println("Check Solar!");

  /* In ADC function */
  led_bat = true;     // Battery = 15.00V+
  bat_stat = true;    // Battery = 16.70V+
  sol_stat = true;    // Solar   = 20.00V+
}

/** Charge battery from Solar **/
void Charge_Bat(void) {
  delay(200);
  delay(2000);
  Serial.println("OLED: CHAGRE ON!");
}

/** LED STRIP (420mA) **/
void Aziz_Light(bool shine) {
  delay(2050);
  Serial.println("OLED: AZIZ ON!");
}