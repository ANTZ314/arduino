/*
Custom OLED + ADC Read (x2) + Push_Button:
  -> LOOP:
    => Wake from sleep mode (every 30 sec?)
    => Check ADC_01 for under/over voltage
    => If bat(<16v75) && solar(>16V8) --> Charge
    => If bat(>15v00) && light(night) --> LEDs On
  -> BUTTON:
    => Relay_1: Read voltage (Battery)            - Display (2sec)
    => Relay_2: Read voltage (Solar_P)            - Display (2sec)
    => Relay_3: Charge Battery from Solar source  - Display (2sec)
    => Relay_4: LED Strip on (light sensitive)    - Display (2sec)
    => Screen Sleep
  -> [LATER] LED Mode Selection:
    => Mode_01: Long slow blink (breathing?)
    => Mode_02: Short Fast blink
    => Mode_03: Full On / Full Off
  -> PINOUTS:
    => A0  - Battery 5V
    => A1  - LDR 5V
    => A2  - Solar 5V
    => D21 - LED Strip Relay
    => D3  - Battery Relay
    => D4  - Solar ADC Relay
    => D9  - Charge Relay
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Arduino UNO:         A4(SDA),  A5(SCL)
// Arduino Nano Every:  D18(SDA), D19(SCL)
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* GPIOs & ADC */
const int ledPin = 13;                          // Output: BUilt-in LED <--- REMOVE
const int Relay1 = 3;                           // Relay: Vbat (ADC)
const int Relay2 = 2;                           // Relay: Vsol (ADC)
const int Relay3 = 4;                           // Relay: Solar Charge
const int Relay4 = 5;                           // Relay: LED Strip
const int buttonPin = 11;                       // Input: Pushbutton pin
const int Bat_Read = A1;                        // ADC: Battery analog read
const int Sol_Read = A2;                        // ADC: Solar analog read
const int LDR_Read = A3;                        // ADC: LDR analog read


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
bool is_day;                                    // Day or Night mode
bool bat_stat;                                  // Battery (bad <= 15.00V)
bool sol_stat;                                  // Solar (bad <= 16.79V)
bool led_bat;                                   // LEDs need (>15.00V)

/**/
void setup() {
  Serial.begin(9600);
  pinMode(Relay1, OUTPUT);                      // init Relay 1
  pinMode(Relay2, OUTPUT);                      // init Relay 2
  pinMode(Relay3, OUTPUT);                      // init Relay 3
  pinMode(Relay4, OUTPUT);                      // init Relay 4
  pinMode(ledPin, OUTPUT);                      // init Onboard LED
  pinMode(buttonPin, INPUT);                    // init pushbutton
  // No need to define ADC pins as inputs?

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));           // (REMOVE) 
    for(;;);                                    // Don't proceed, loop forever
  }
  // Show initial display buffer
  display.display();                            // (REMOVE)
  delay(2000);                                  // Pause for 2 seconds
  display.clearDisplay();                       // Clear the buffer
  //testdrawstyles();                             // Draw 'stylized' characters

}

/** MAIN LOOP **/
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

  /** SLEEP MODE */
  // sleep();
  delay(10000) ;                                // Delay 10 sec (60 sec later)
  /** NEEDS TO BE CAUGHT EVERY 30 SECONDS **/
  user_check();                                 // Check button & display voltages
  delay(10000) ;                                // Delay 10 sec (60 sec later)
}

/** User Status Check **/
void user_check(void) {
  buttonState = digitalRead(buttonPin);         // read pushbutton value

  /* Button Pressed: Display Voltages */
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);                 // turn on-board LED on   <-- REMOVE
    delay(500);                                 // Debounce press
    Power_Check();                              // Read Battery voltage
    oled_display(0);                            // Display Battery Voltage
    delay(1500);                                // 1.5 sec
    oled_display(1);                            // Display Solar Voltage
    delay(1500);
    // Display Mode
    digitalWrite(ledPin, LOW);                  // turn on-board LED off  <-- REMOVE
  }
}

/** Read LDR light sensitiviy **/
void day_night(void) {
  ldrADC = analogRead(LDR_Read);                  // read the input on analog pin 0 (D15)
  ldr_sens = ldrADC * (3.3 / 1023.0);             // analog (0 - 1023) to voltage (0 - 3V3)

  if(ldr_sens <= 1.8){ is_day = false; }          // is_night time
  else{ is_day = true; }                          // is_day time

  /* REMOVE PRINT */
  if(ldr_sens <= 1.8){ Serial.println("Night"); } // REMOVE!
  else{ Serial.println("Day"); }                  // REMOVE!
}                                                 // */

/** Check each power supply voltage **/
void Power_Check(void){
  /* All Off */
  digitalWrite(Relay1, LOW);                      // Open Relay1: Battery
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2: Solar
  delay(100);
  digitalWrite(Relay3, LOW);                      // Open Relay3: Charge
  delay(100);
  /* BATTERY */
  digitalWrite(Relay1, HIGH);                     // Close Relay1: Battery
  delay(100);
  /* Check Battery Voltage */
  adc_01_read(true);                              // Read Battery voltage
  delay(200);
  digitalWrite(Relay1, LOW);                      // Close Relay1
  delay(100);
  /* SOLAR */
  digitalWrite(Relay2, HIGH);                     // Open Relay2: Solar
  delay(100);
  /* Check Solar Voltage */
  adc_01_read(false);                             // Read Solar voltage
  delay(200);
  /* BOTH CLOSED */
  digitalWrite(Relay1, LOW);                      // Close Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                      // Close Relay2
  delay(100);
  /*________________________________________*/
  /** NOTE: Convert back to real voltages! **/
  /*________________________________________*/
}

/** Read Solar & Battery voltage levels **/
void adc_01_read(bool bat_sol) {

  if(bat_sol == true){
    powerADC = analogRead(Bat_Read);              // read the input on analog pin 0 (D14)
    voltage1 = powerADC * (5.0 / 1023.0);         // Convert analog (0 - 1023) to voltage (0 - 5V)

    Bat_Volt = voltage1;                          // Store battery voltage float = float
    Act_Bat_V = Bat_Volt * 3.36;                  // Convert to voltage (5V x 3.36 = 16V8)
   
    Serial.print("Battery2: ");                   // REMOVE!
    Serial.println(Act_Bat_V);                    // REMOVE!
  }
  else if(bat_sol == false){
    powerADC = analogRead(Sol_Read);              // read the input on analog pin 0 (D14)
    voltage2 = powerADC * (5.0 / 1023.0);         // Convert analog (0 - 1023) to voltage (0 - 5V)

    Sol_Volt = voltage2;                          // Store solar panel voltage
    Act_Sol_V = Sol_Volt * 4.06;                  // Convert to actual voltage (5V x 4.27 = 21V35)
    
    Serial.print("Solar2: ");                     // REMOVE!
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

  /* REMOVE PRINT * /
  if(Act_Bat_V >= 15.00){ Serial.println("LEDS ON"); } // REMOVE
  else{ Serial.println("LEDS OFF"); }
  /* Battery Charge Status * /
  if(Act_Bat_V >= 16.70){"BAT GOOD"}              // REMOVE
  else{"BAT BAD"}
  /* Solar Charge Status * /
  if(Act_Sol_V >= 20){"SOL GOOD"}                 // REMOVE
  else{"SOL BAD"}                                 // REMOVE
  */
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
    Serial.println("CHARGE ON");                  //                      <--- REMOVE!
    oled_display(2);                              // OLED Display "Charge Mode"
    delay(750);
    oled_display(4);                              // OLED Display Off

    digitalWrite(Relay2, HIGH);                   // Close Solar (Relay2)
    delay(100);                                   // 100ms delay
    digitalWrite(Relay3, HIGH);                   // Close Charge (Relay3)
    //delay(5000);                                  // Charge for 5 sec     <--- REMOVE 
    //digitalWrite(Relay3, LOW);                    // Open Charge (Relay3) <--- REMOVE
  }
  /* Don't Charge */
  else{   
    Serial.println("CHARGE OFF");                 //                      <--- REMOVE!
    digitalWrite(Relay3, LOW);                    // Open Relay3: Charge
    delay(100);
  }
}

/** LED Strip Active **/
void Aziz_Light(bool shine) {

  /*~~~~~~ONLY IF BATTERY GOOD~~~~~~~*/

  /* Lights On */
  if(shine == true){
    Serial.println("AZIZ ON");                    //                      <--- REMOVE!
    oled_display(3);                              // OLED Display: "Aziz Light!"
    delay(750);
    oled_display(4);                              // OLED Display Off

    digitalWrite(LED_BUILTIN, HIGH);              // On (REMOVE)
    digitalWrite(Relay4, HIGH);                   // Close Relay4 (ON)
    delay(100);
  }
  /* Lights Off */
  else if(shine == false){
    Serial.println("AZIZ OFF");                   //                      <--- REMOVE!
    digitalWrite(LED_BUILTIN, LOW);               // Off (REMOVE)
    digitalWrite(Relay4, LOW);                    // Open Relay4 (OFF)
    delay(100);
  }
}

/** Display current voltage levels **/
void oled_display(int bat_sol) {
  /* Skip if Solar Value Invalid */
  if (bat_sol < 0 || bat_sol > 4) {
    Serial.println(F("ERROR"));                 // Value Invalid <-- REMOVE
    return; // Exit if bat_sol is invalid
  }
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
    display.println(F("Charge Mode:"));       // Charge Mode
  }
  else if(bat_sol == 3){
    display.println(F("LED Mode:"));       // LED Mode
  }

  display.setTextColor(SSD1306_WHITE);          // Draw white text
  display.println("------------------");

  display.setTextSize(2);                       // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  /* Battery Voltage */
  if(bat_sol == 0){
    display.print(Act_Bat_V);                   // 16.8V / 3.36 = 5V
    display.println(F("V"));
  }
  /* Solar Voltage */
  else if(bat_sol == 1){
    display.print(Act_Sol_V);                   // 21.4V / 4.28 = 5V
    display.println(F("V"));
  }
  /* Charge Mode */
  else if(bat_sol == 2){ 
    display.println(F("--SOLAR--"));         // Charge On/Off
  }
  /* LED Mode */
  else if(bat_sol == 3){
    display.println(F("AZIZ!!"));               // LEDs On/Off
  }

  display.display();
  delay(2000);                                  // Display voltage for 2sec
  display.clearDisplay();

  if(bat_sol == 4){
    Serial.println("Screen Off");               //                <-- REMOVE!
    display.ssd1306_command(SSD1306_DISPLAYOFF);  // Sleep the Display
  }
}
