/*
Integrate "sleep_int.c" with "solar_ctrl5.c"
Full control code with interrupt wake up D2
*/

#include <avr/sleep.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

uint8_t loopCount = 0 ;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* GPIOs & ADC */
const int Relay1 = 3;                           // Battery Relay (ADC)
const int Relay2 = 4;                           // Solar Panel Relay (ADC)
const int Relay3 = 9;                           // Solar Relay (Charge)
const int Relay4 = 21;                          // LED Strip
const int buttonPin = 2;                       // Pushbutton pin (was 11)
const int ledPin = 13;                          // BUilt-in LED pin <--- REMOVE
const int Bat_Read = A0;                        // Battery analog read
const int LDR_Read = A1;                        // LDR analog read
const int Sol_Read = A2;                        // Solar analog read


/* Global variables */
int powerADC;                                   // ADC1 - Both power voltage
int ldrADC;                                     // ADC2 - LDR sensor
float ldr_sens;                                 // converted ldr value
float voltage1;                                 // converted voltages
float voltage2;                                 // converted voltages
float Bat_Volt  = 0.00;                         // Battery voltage
float Sol_Volt  = 0.00;                         // Solar Panel voltage
float Act_Bat_V = 0.00;                         // Actual voltage (converted)
float Act_Sol_V = 0.00;                         // Actual voltage (converted)
float LDR_Volt  = 0.00;                         // LDR Sensor voltage
int buttonState = 0;                            // pushbutton status
int chrg_mode   = 0;                            // oled charge message
bool is_day;                                    // Day or Night mode
bool bat_stat;                                  // Battery (bad <= 15.00V)
bool sol_stat;                                  // Solar (bad <= 16.79V)
bool led_bat;                                   // LEDs need (>15.00V)
int slp_cnt = 0;                                // sleep counter

/* MAIN SETUP */
void setup() {
  Serial.begin(9600);
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Relay1, OUTPUT);                      // init Relay 1
  pinMode(Relay2, OUTPUT);                      // init Relay 2
  pinMode(Relay3, OUTPUT);                      // init Relay 3
  pinMode(Relay4, OUTPUT);                      // init Relay 4
  pinMode(ledPin, OUTPUT);                      // init Onboard LED
  pinMode(buttonPin, INPUT);                    // init pushbutton

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));           // (REMOVE) 
    for(;;);                                    // Don't proceed, loop forever
  }
  // Show initial display buffer
  display.display();                            // Adafruit Welcome Logo (REMOVE/CHANGE)
  delay(2000);                                  // Pause for 2 seconds
  //oled_display(5);                              // OLED Display Off
  Serial.println("Setup Complete");
}

/* MAIN LOOP */
void loop() {
  /** NEEDS TO BE CAUGHT ON WAKE-UP **/
  user_check();                                 // Check button & display voltages

  /* Check Day/Night mode -- ON WAKE-UP (Every minute?) */
  day_night();                                  // Is it day or night?

  /*Check Battery & Solar voltages */
  Power_Check();                                // Check battery (and solar)

  /** ~~~~~~~~NIGHT~~~~~~~~ **/
  if(is_day == false){                          // Range: Bright(2.8V) - Dim(1.3V)
    /* Night && Battery == good */
    if(led_bat == true){
      Aziz_Light(true);                         // Lights On
    }
    /* Night && Battery == bad  */
    else{Aziz_Light(false);}                    // Lights Off
  }
  /** ~~~~~~~~~DAY~~~~~~~~~ **/
  else if(is_day == true){
    Aziz_Light(false);                          // Lights Off
    Charge_Bat();                               // Decide whether to charge
  }

  /** Sleep after 5 loops **/
  sleep_cnt();
}

void sleep_cnt(void) {
  slp_cnt++;
  if(slp_cnt == 5){
    slp_cnt = 0;
    Sleep_CPU();                                // Call function to sleep CPU
  }
}

void Sleep_CPU(void) {
  Serial.flush();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);          // Sets the sleep mode
  cli();
  sleep_enable();                               // set sleep bit
  attachInterrupt(digitalPinToInterrupt(buttonPin), Wake_CPU, RISING); // attach interrupt to wake CPU after sleep. 
  //digitalWrite(ledPin, LOW);                  // turn off LED to show sleep mode
  sei();
  sleep_cpu();                                  // put to sleep - will wake up here. 
  sleep_disable();
  Serial.println("-AWAKE-");                    // first line of code executed after sleep. 
  //digitalWrite(ledPin, HIGH);                 //turning LED on
}

void Wake_CPU(void) {
  detachInterrupt(digitalPinToInterrupt(buttonPin));  //Removes the interrupt from pin 2;
}

/** User Status Check **/
void user_check(void) {
  buttonState = digitalRead(buttonPin);         // read pushbutton value

  /* Button pressed */
  if (buttonState == HIGH) {
    //Serial.println("- BUTTON -");
    //digitalWrite(ledPin, HIGH);                 // turn on-board LED on   <-- REMOVE
    
    delay(500);                                   // Debounce press
    Power_Check();                                // Read Battery voltage
    oled_display(0);                            // Display Battery Voltage
    delay(1500);                                  // 1.5 sec
    oled_display(1);                            // Display Solar Voltage
    delay(1500);
    oled_display(chrg_mode);                    // Display Solar Voltage
    delay(1500);
    oled_display(5);                            // OLED Display Off

    //digitalWrite(ledPin, LOW);                  // turn on-board LED off  <-- REMOVE
  }
}

/** Read LDR light sensitiviy **/
void day_night(void) {
  ldrADC = analogRead(LDR_Read);                  // read the input on analog pin 0 (D15)
  ldr_sens = ldrADC * (3.3 / 1023.0);             // analog (0 - 1023) to voltage (0 - 3V3)
  ldr_sens = 1.9;                                 //  <-- REMOVE

  if(ldr_sens <= 1.8){ is_day = false; }          // is_night time
  else{ is_day = true; }                          // is_day time
}                                                 // */

/** Check each power supply voltage **/
void Power_Check(void) {  
  digitalWrite(Relay1, LOW);                      // Open Relay1: Battery
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2: Solar
  delay(100);
  digitalWrite(Relay3, LOW);                      // Open Relay3: Charge
  delay(100);

  digitalWrite(Relay1, HIGH);                     // Close Relay1: Battery
  delay(100);
  /* Check Battery Voltage */
  adc_01_read(true);                              // Read Battery voltage
  delay(200);
  digitalWrite(Relay1, LOW);                      // Close Relay1
  delay(100);

  digitalWrite(Relay2, HIGH);                     // Open Relay2: Solar
  delay(100);
  /* Check Solar Voltage */
  adc_01_read(false);                             // Read Solar voltage
  delay(200);

  digitalWrite(Relay1, LOW);                      // Close Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                      // Close Relay2
  delay(100);
}

/** Read Solar & Battery voltage levels **/
void adc_01_read(bool bat_sol) {

  if(bat_sol == true){
    //powerADC = analogRead(Bat_Read);              // read the input on analog pin 0 (D14)
    //voltage1 = powerADC * (5.0 / 1023.0);         // Convert analog (0 - 1023) to voltage (0 - 5V)

    Bat_Volt = voltage1;                          // Store battery voltage float = float
    Act_Bat_V = Bat_Volt * 3.36;                  // Convert to voltage (5V x 3.36 = 16V8)
   
    Act_Bat_V = 16.75;                            // charge < 16.70 < don't   <-- REMOVE
    Serial.print("Battery: ");                    //                          <-- REMOVE!
    Serial.println(Act_Bat_V);                    //                          <-- REMOVE!
  }
  else if(bat_sol == false){
    //powerADC = analogRead(Sol_Read);              // read 2the input on analog pin 0 (D14)
    //voltage2 = powerADC * (5.0 / 1023.0);         // Convert analog (0 - 1023) to voltage (0 - 5V)

    Sol_Volt = voltage2;                          // Store solar panel voltage
    Act_Sol_V = Sol_Volt * 4.06;                  // Convert to actual voltage (5V x 4.27 = 21V35)
    
    Act_Sol_V = 20.20;                            // don't < 20.20 < charge   <-- REMOVE
    Serial.print("Solar: ");                      //                          <-- REMOVE!
    Serial.println(Act_Sol_V);                    //                          <-- REMOVE!
  }

  /* Light Battery Status */
  if(Act_Bat_V >= 15.00){ led_bat = true; }       // LEDs on above if battery is 15.00V+
  else{ led_bat = false; }                        // Battery min 14.80V
  /* Battery Charge Status */
  if(Act_Bat_V >= 16.70){ bat_stat = true; }      // Don't Charge battery over 16.70V
  else{ bat_stat = false; }                       // Charge battery up to 16.69V
  /* Solar Charge Status */
  if(Act_Sol_V >= 20){ sol_stat = true; }         // Charge if solar is 16.80V+
  else{ sol_stat = false; }                       // 20.3V stepped down to 16.8V
}

/** Charge battery from Solar **/
/** NOTE: Solar 21.4Vmax stepped down to 16V8 before ADC **/
void Charge_Bat(void) {
  digitalWrite(Relay1, LOW);                      // Open Relay1: Battery
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2: Solar
  delay(100);

  /* Charge Battery */
  if (bat_stat == false && sol_stat == true){     // (bat < 16V7) && (sol > 16V8)
    Serial.println("CHARGE!");                    //                      <--- REMOVE!
    chrg_mode = 2;                                // oled charge message

    digitalWrite(Relay3, HIGH);                   // Close Relay1
    delay(5000);                                  // Charge for 5 sec     <--- REMOVE
    digitalWrite(Relay3, LOW);                    // Open Relay1          <--- REMOVE
  }
  /* Don't Charge */
  else{
    Serial.println("DON'T CHARGE!");              //                      <--- REMOVE!
    chrg_mode = 3;                                // oled charge message
    digitalWrite(Relay3, LOW);                    // Open Relay2: Solar
    delay(100);
  }
}

/** LED Strip Active **/
void Aziz_Light(bool shine) {

  /*~~~~~~ONLY IF BATTERY GOOD~~~~~~~*/

  /* Lights On */
  if(shine == true){
    chrg_mode = 4;                                //                     <--- REMOVE!
    Serial.println("Lights On!");                 // 

    digitalWrite(LED_BUILTIN, HIGH);              // On (REMOVE)
    digitalWrite(Relay4, HIGH);                   // Close Relay4 (ON)
    delay(100);
  }
  /* Lights Off */
  else if(shine == false){
    Serial.println("Lights Off!");                 //                     <--- REMOVE! 
    //chrg_mode = 4;                                // 
    digitalWrite(LED_BUILTIN, LOW);               // Off (REMOVE)
    digitalWrite(Relay4, LOW);                    // Open Relay4 (OFF)
    delay(100);
  }
}

/** Display current volatge levels **/
void oled_display(int bat_sol) {
  display.ssd1306_command(SSD1306_DISPLAYON);   // Wake-Up Display
  display.clearDisplay();

  display.setTextSize(1);                       // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);          // Draw white text
  display.setCursor(0,0);                       // Start at top-left corner
  if(bat_sol == 0){
    display.println(F("Battery Voltage:"));     // Battery Voltage
  }
  else if(bat_sol == 1){
    display.println(F("Solar Voltage:"));       // Solar Voltage
  }
  else if(bat_sol == 2){
    display.println(F("Charge State:"));        // Charge Mode: ON
  }
  else if(bat_sol == 3){
    display.println(F("Charge State:"));        // Charge Mode: OFF
  }
  else if(bat_sol == 4){
    display.println(F("Night Mode:"));          // LED Mode
  }
  else if(bat_sol == 5){
    display.println(F("-- Good Bye --"));        // LED Mode
  }

  display.setTextColor(SSD1306_WHITE);          // Draw white text
  display.println("------------------");

  display.setTextSize(2);                       // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  if(bat_sol == 0){
    display.print(Act_Bat_V);                   // 16.8V / 3.36 = 5V
    display.println(F("V"));
  }
  else if(bat_sol == 1){
    display.print(Act_Sol_V);                   // 21.4V / 4.28 = 5V
    display.println(F("V"));
  }
  else if(bat_sol == 2){
    display.println(F("- ON -"));               // Charge: ON
  }
  else if(bat_sol == 3){
    display.println(F("- OFF -"));              // Charge: OFF
  }
  else if(bat_sol == 4){
    display.println(F("Aziz!!"));               // Lights
  }
  else if(bat_sol == 5){
    display.println(F("- ADIOS -"));            // Bye
  }

  display.display();
  delay(2000);                                  // Display voltage for 2sec
  display.clearDisplay();

  if(bat_sol == 5){
    Serial.println("Screen Off");                 //                <-- REMOVE!
    display.ssd1306_command(SSD1306_DISPLAYOFF);  // Sleep the Display
  }
}
