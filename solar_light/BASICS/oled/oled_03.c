/*
Custom OLED + ADC Read (x2) + Push_Button:
  -> Continuously check ADC_01 for under/over voltage:
    => Wake from sleep mode (every 30 sec?)
    => If bat(<16v75) && solar(>16V8) --> Charge
    => If bat(>15v00) && light(night) --> LEDs On
  -> Button Press:
    => Relay_1: Read voltage (Battery)            - Display (2sec)
    => Relay_2: Read voltage (Solar_P)            - Display (2sec)
    => Relay_3: Charge Battery from Solar source  - Display (2sec)
    => Relay_4: LED Strip on (light sensitive)    - Display (2sec)
    => Screen Sleep
  -> PINOUTS:
    => A0  - Battery 5V
    => A1  - LDR 5V
    => A2  - Solar 5V
    => D21 - LED Strip Relay
    => D3  - Battery Relay
    => D4  - Solar ADC Relay
    => D9  - Charge Relay
  -> [LATER] LED Mode Selection:
    => Mode_01: Long slow blink (breathing?)
    => Mode_02: Short Fast blink
    => Mode_03: Full On / Full Off
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Arduino UNO:         A4(SDA),  A5(SCL)
// Arduino Nano Every:  D18(SDA), D19(SCL)
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* GPIOs & ADC */
const int Relay1 = 3;                           // Battery Relay (ADC)
const int Relay2 = 4;                           // Solar Panel Relay (ADC)
const int Relay3 = 9;                           // Solar Relay (Charge)
const int Relay4 = 21;                          // LED Strip
const int buttonPin = 2;                       // Pushbutton pin
const int ledPin = 13;                          // BUilt-in LED pin <--- REMOVE
const int Bat_Read = A0;                        // Battery analog read
const int LDR_Read = A1;                        // LDR analog read
const int Sol_Read = A2;                        // Solar analog read
uint8_t loopCount = 0 ;                         // For the Sleep Routine


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

/*  Sleep Mode & it's interrupt * /
void RTC_init(void)
{
  while (RTC.STATUS > 0) ;                      // Wait for all register to be synchronized
 
  RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;             // Run low power oscillator (OSCULP32K) at 1024Hz for long term sleep
  RTC.PITINTCTRL = RTC_PI_bm;                   // PIT Interrupt: enabled
  /* Set period 8 seconds (see data sheet) and enable PIC * /
  RTC.PITCTRLA = RTC_PERIOD_CYC8192_gc | RTC_PITEN_bm;
}

ISR(RTC_PIT_vect)
{
  RTC.PITINTFLAGS = RTC_PI_bm;                  // Clear interrupt flag by writing '1' (required) 
}                                               // */

/**/
void setup() {
  Serial.begin(9600);
  pinMode(Relay1, OUTPUT);                      // init Relay 1
  pinMode(Relay2, OUTPUT);                      // init Relay 2
  pinMode(Relay3, OUTPUT);                      // init Relay 3
  pinMode(Relay4, OUTPUT);                      // init Relay 4
  pinMode(ledPin, OUTPUT);                      // init Onboard LED
  pinMode(buttonPin, INPUT);                    // init pushbutton
  /* No need to define ADC pins as inputs?? */

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));           // (REMOVE) 
    for(;;);                                    // Don't proceed, loop forever
  }
  // Show initial display buffer
  display.display();                            // Adafruit Welcome Logo (REMOVE/CHANGE)
  delay(2000);                                  // Pause for 2 seconds
  oled_display(5);                              // OLED Display Off
}

void loop() {
  /** NEEDS TO BE CAUGHT EVERY 30 SECONDS **/
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

  delay(10000) ;                              // Delay 10 sec (60 sec later)
  user_check();                                 // Check button & display voltages
  delay(10000) ;                              // Delay 10 sec (60 sec later)
  /** SLEEP MODE * /
  // sleep();
  sleep_time();                                 // Go to Sleep

  /** NEEDS TO BE CAUGHT EVERY 30 SECONDS ** /
  Serial.println("AWAKE...");
  user_check();                                 // Check button & display voltages
  //delay(10000) ;                              // Delay 10 sec (60 sec later)
  sleep_time();                                 // Go to Sleep
  Serial.println("AWAKE AGAIN...");             // */
}
/*  * /
void sleep_time(void) {
  Serial.println("SLEEP...");
  RTC_init();                                   // Initialise Sleep Timers
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);          // Set sleep mode to POWER DOWN mode 
  sleep_enable();                               // Enable sleep mode, but not going to sleep yet
}                                               // */

/** User Status Check **/
void user_check(void) {
  buttonState = digitalRead(buttonPin);         // read pushbutton value

  /* Button pressed */
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);                 // turn on-board LED on   <-- REMOVE
    delay(500);                                 // Debounce press
    Power_Check();                              // Read Battery voltage
    oled_display(0);                            // Display Battery Voltage
    delay(1500);                                // 1.5 sec
    oled_display(1);                            // Display Solar Voltage
    delay(1500);
    oled_display(chrg_mode);                    // Display Solar Voltage
    delay(1500);
    oled_display(5);                            // OLED Display Off
    // Display Mode
    digitalWrite(ledPin, LOW);                  // turn on-board LED off  <-- REMOVE
  }
}

/** Read LDR light sensitiviy **/
void day_night(void) {
  //ldrADC = analogRead(LDR_Read);                  // read the input on analog pin 0 (D15)
  //ldr_sens = ldrADC * (3.3 / 1023.0);             // analog (0 - 1023) to voltage (0 - 3V3)
  //ldr_sens = 1.9;   // Day
  ldr_sens = 1.7;   // Night

  if(ldr_sens <= 1.8){ is_day = false; }          // is_night time
  else{ is_day = true; }                          // is_day time
}                                                 // */

/** Check each power supply voltage **/
void Power_Check(void){
  //Serial.print("--POWER CHECK: ");
  delay(100);
  delay(100);
  delay(100);

  delay(100);
  /* Check Battery Voltage */
  //Serial.print("Battery Check | ");
  adc_01_read(true);
  delay(200);
  delay(100);

  delay(100);
  /* Check Solar Voltage */
  //Serial.println("Solar Check");
  adc_01_read(false);
  delay(200);

  delay(100);
  delay(100);
  /*________________________________________*/
  /** NOTE: Convert back to real voltages! **/
  /*________________________________________*/
}

/** Read Solar & Battery voltage levels **/
void adc_01_read(bool bat_sol) {

  if(bat_sol == true){
    //powerADC = analogRead(Bat_Read);              // read the input on analog pin 0 (D14)
    //voltage1 = powerADC * (5.0 / 1023.0);         // Convert analog (0 - 1023) to voltage (0 - 5V)
    voltage1 = 4.7;

    Bat_Volt = voltage1;                          // Store battery voltage float = float
    Act_Bat_V = Bat_Volt * 3.36;                  // Convert to voltage (5V x 3.36 = 16V8)
   
    Serial.print("Battery: ");                    // REMOVE!
    Serial.println(Act_Bat_V);                    // REMOVE!
  }
  else if(bat_sol == false){
    //powerADC = analogRead(Sol_Read);              // read the input on analog pin 0 (D14)
    //voltage2 = powerADC * (5.0 / 1023.0);         // Convert analog (0 - 1023) to voltage (0 - 5V)
    voltage2 = 4.93;    // Charge
    //voltage2 = 4.85;  // Don't Charge

    Sol_Volt = voltage2;                          // Store solar panel voltage
    Act_Sol_V = Sol_Volt * 4.06;                  // Convert to actual voltage (5V x 4.27 = 21V35)
    
    Serial.print("Solar: ");                      // REMOVE!
    Serial.println(Act_Sol_V);                    // REMOVE!
  }

  /* Light Battery Status */
  if(Act_Bat_V >= 15.00){ led_bat = true; }       // LEDs on above if battery is 15.00V+
  else{ led_bat = false; }
  /* Battery Charge Status */
  if(Act_Bat_V >= 16.70){ bat_stat = true; }      // Charge battery up to 16.75V
  else{ bat_stat = false; }
  /* Solar Charge Status */
  if(Act_Sol_V >= 20){ sol_stat = true; }         // Charge if solar is 16.80V+
  else{ sol_stat = false; }                       // 20.3V stepped down to 16.8V
}

/** Charge battery from Solar **/
/** NOTE: Solar 21.4Vmax stepped down to 16V8 before ADC **/
void Charge_Bat(void) {

  /* Charge Battery */
  if (bat_stat == false && sol_stat == true){     // (bat < 16V7) && (sol > 16V8)
    Serial.println("CHARGE!");                    //                      <--- REMOVE!
    chrg_mode = 2;                                // oled charge message
  }
  /* Don't Charge */
  else{   
    Serial.println("DON'T CHARGE!");              //                      <--- REMOVE!
    chrg_mode = 3;                                // oled charge message
  }
}

/** LED Strip Active **/
void Aziz_Light(bool shine) {

  /*~~~~~~ONLY IF BATTERY GOOD~~~~~~~*/

  /* Lights On */
  if(shine == true){
    chrg_mode = 4;                                // 
    Serial.println("Lights On!");                 // (REMOVE)
    digitalWrite(LED_BUILTIN, HIGH);              // On (REMOVE)
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);               // On (REMOVE)
  }
  /* Lights Off */
  else if(shine == false){
    //chrg_mode = 4;                              // 
    digitalWrite(LED_BUILTIN, LOW);               // Off (REMOVE)
    Serial.println(F("Lights Off!"));
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
    display.println(F("Charge State:"));        // Charge Mode
  }
  else if(bat_sol == 3){
    display.println(F("Charge State:"));        // LED Mode
  }
  else if(bat_sol == 4){
    display.println(F("Night Mode:"));          // LED Mode
  }
  else if(bat_sol == 5){
    display.println(F("-- Good Bye --"));        // LED Mode
  }

  display.setTextColor(SSD1306_WHITE);          // Draw white text
  display.println("--------------------");

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
    display.println(F("- ON -"));               // Charge ON
  }
  else if(bat_sol == 3){
    display.println(F("- OFF -"));              // Charge OFF
  }
  else if(bat_sol == 4){
    display.println(F("Aziz!!"));               // Lights
  }
  else if(bat_sol == 5){
    display.println(F("- ADIOS -"));            // Lights
  }

  display.display();
  delay(2000);                                  // Display voltage for 2sec
  display.clearDisplay();

  if(bat_sol == 5){
    Serial.println("Screen Off");                 //                <-- REMOVE!
    display.ssd1306_command(SSD1306_DISPLAYOFF);  // Sleep the Display
  }
}
