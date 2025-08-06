/*
Battery charge test:
  -> Check battery voltage (Relay_1)
  -> Check solar voltage   (Relay_2)
  -> If: (battery <= 15V) && (solar >= 16.8V)
    => Charge battery      (Relay_3)
  -> PINOUTS:
    => A0  - Battery 5V
    => A2  - Solar 5V
    => D3  - Battery Relay
    => D4  - Solar ADC Relay
    => D9  - Charge Relay
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* GPIOs & ADC */
const int Relay1 = 3;                           // Battery Relay (ADC)
const int Relay2 = 4;                           // Solar Panel Relay (ADC)
const int Relay3 = 9;                           // Solar Relay (Charge)
const int ledPin = 13;                          // BUilt-in LED pin <--- REMOVE
const int Bat_Read = A0;                        // Battery analog read
const int Sol_Read = A2;                        // Solar analog read


/* Global variables */
int powerADC;                                   // ADC1 - Both power voltage
float voltage1;                                 // converted voltages
float voltage2;                                 // converted voltages
float Bat_Volt  = 0.00;                         // Battery voltage
float Sol_Volt  = 0.00;                         // Solar Panel voltage
float Act_Bat_V = 0.00;                         // Actual voltage (converted)
float Act_Sol_V = 0.00;                         // Actual voltage (converted)
bool bat_stat;                                  // Battery (bad <= 15.00V)
bool sol_stat;                                  // Solar (bad <= 16.79V)

/**/
void setup() {
  Serial.begin(9600);
  pinMode(Relay1, OUTPUT);                      // init Relay 1
  pinMode(Relay2, OUTPUT);                      // init Relay 2
  pinMode(Relay3, OUTPUT);                      // init Relay 3
  pinMode(ledPin, OUTPUT);                      // init Onboard LED
}

void loop() {

  /*Check Battery & Solar voltages */
  Power_Check();                                  // Check battery (and solar)

  Charge_Bat();                                   // Decide whether to charge

  //delay(10000) ;                                  // Delay 10 sec (60 sec later)
}

/** Check each power supply voltage **/
void Power_Check(void){  
  digitalWrite(Relay1, LOW);                      // Open Relay1: Battery
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2: Solar
  delay(100);
  digitalWrite(Relay3, LOW);                      // Open Relay3: Charge
  delay(100);

  digitalWrite(Relay1, HIGH);                     // Close Relay1: Battery
  delay(200);
  /* Check Battery Voltage */
  adc_01_read(true);                              // Read Battery voltage
  delay(200);
  digitalWrite(Relay1, LOW);                      // Close Relay1
  delay(200);

  digitalWrite(Relay2, HIGH);                     // Open Relay2: Solar
  delay(200);
  /* Check Solar Voltage */
  adc_01_read(false);                             // Read Solar voltage
  delay(200);

  digitalWrite(Relay1, LOW);                      // Open Relay1
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2
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

  /* Battery Charge Status */
  if(Act_Bat_V >= 16.70){ bat_stat = true; }      // Charge battery up to 16.75V
  else{ bat_stat = false; }
  /* Solar Charge Status */
  if(Act_Sol_V >= 20){ sol_stat = true; }         // Charge if solar is greater than 20V
  else{ sol_stat = false; }                       // 20.3V stepped down to 16.8V
}

/** Charge battery from Solar **/
/** NOTE: Solar 21.4Vmax stepped down to 16V8 **/
void Charge_Bat(void) {
  digitalWrite(Relay1, LOW);                      // Open Relay1: Battery
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2: Solar
  delay(100);
  digitalWrite(Relay3, LOW);                      // Open Relay2: Solar
  delay(100);

  /* Charge Battery */
  if (bat_stat == false && sol_stat == true){     // (bat < 16V7) && (sol > 16V8)

    Serial.println("CHARGE!");                    // REMOVE!
  
    digitalWrite(Relay3, HIGH);                   // Close Relay1
    delay(5000);                                  // Charge for 5 sec
    digitalWrite(Relay3, LOW);                    // Open Relay1
  }
}