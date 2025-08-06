#include <TimerOne.h>

const int buttonPin = 2;  // Example button pin
const int ledPin = LED_BUILTIN;
int buttonState = LOW;
int lastButtonState = LOW;

// State machine for display sequence
enum DisplayState { IDLE, SHOW_BATTERY, SHOW_SOLAR };
DisplayState displayState = IDLE;

// State machine for Action2 sequence
enum Action2State { A2_IDLE, A2_STEP1, A2_STEP2 };
Action2State action2State = A2_IDLE;

// Counter for 60-second interval (12 * 5 seconds)
volatile int timerCount = 0;
const int countsFor60Seconds = 12; // 60s / 5s = 12
volatile bool triggerAction2 = false; // Flag to trigger Action2 in loop()

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Initialize Timer1 for 5-second interrupts
  Timer1.initialize(5000000); // 5 seconds in microseconds
  Timer1.attachInterrupt(timerISR);
}

void timerISR() {
  // Action1: Executes every 5 seconds
  Serial.println("Action1: 5 seconds elapsed!");
  // Place Action1 code here

  // Increment counter for 60-second actions
  timerCount++;
  if (timerCount >= countsFor60Seconds) {
    triggerAction2 = true; // Signal Action2 to start in loop()
    Serial.println("Action3: 60 seconds elapsed!");
    // Place Action3 code here (e.g., Power_Check())
    timerCount = 0;
  }
}

void loop() {
  user_check();  // Handle button and display sequence
  handleAction2(); // Handle Action2 sequence non-blocking
}

void handleAction2() {
  static unsigned long lastAction2Time = 0;
  unsigned long currentMillis = millis();

  if (triggerAction2) {
    switch (action2State) {
      case A2_IDLE:
        // Start Action2 sequence
        Serial.println("Action2: Starting Step 1");
        // Place Action2 Step 1 code here (e.g., day_night())
        action2State = A2_STEP1;
        lastAction2Time = currentMillis;
        break;

      case A2_STEP1:
        // Wait 1 second non-blocking before Step 2
        if (currentMillis - lastAction2Time >= 1000) {
          Serial.println("Action2: Starting Step 2");
          // Place Action2 Step 2 code here
          action2State = A2_STEP2;
          lastAction2Time = currentMillis;
        }
        break;

      case A2_STEP2:
        // Wait 1 second non-blocking to complete
        if (currentMillis - lastAction2Time >= 1000) {
          Serial.println("Action2: Completed");
          action2State = A2_IDLE;
          triggerAction2 = false; // Reset for next 60-second trigger
        }
        break;
    }
  }
}

void user_check() {
  buttonState = digitalRead(buttonPin);         // read pushbutton value

  /* Button pressed */
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);                 // turn on-board LED on   <-- REMOVE
    Power_Check();                              // Read Battery voltage
    oled_display(0);                            // Display Battery Voltage
    delay(1500);                                // 1.5 sec
    oled_display(1);                            // Display Solar Voltage
    delay(1500);                                // 1.5 sec
    digitalWrite(ledPin, LOW);                  // turn on-board LED off  <-- REMOVE
  }
}

// Placeholder functions (replace with your implementations)
void day_night() {
  Serial.println("Checking Day/Night mode...");
}

void Power_Check() {
  Serial.println("Checking Power...");
}

void oled_display(int mode) {
  if (mode == 0) {
    Serial.println("Displaying Battery Voltage");
  } else {
    Serial.println("Displaying Solar Voltage");
  }
}