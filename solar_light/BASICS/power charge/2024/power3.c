/*
Check Battery/Solar voltages --> Test Charging
*/
#include <Wire.h>

/* GPIOs & ADC */
const int Relay1 = 3;                           // Battery Relay (ADC)
const int Relay2 = 4;                           // Solar Panel Relay (ADC)
const int Relay3 = 9;                           // Solar Relay (Charge)
const int Bat_Read = A0;                        // Battery analog read
const int Sol_Read = A2;                        // Solar analog read

/* Global variables */
float powerADC;                                 // ADC1 - Both power voltage
float voltage1;                                 // converted voltages
float voltage2;                                 // converted voltages
float Bat_Volt  = 0.00;                         // Battery voltage
float Sol_Volt  = 0.00;                         // Solar Panel voltage
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

}

void loop() {
  /* Check Battery Voltage */
  adc_01_read();                                // Read Battery voltage
  delay(2000);
  /* Check Solar Voltage */
  adc_02_read();                                // Read Solar voltage
  delay(2000);
}

/** READ BATTERY **/
void adc_01_read(void) {
  voltage1 = 0.00;
  Bat_Volt = 0.00;

  digitalWrite(Relay1, LOW);                      // Open Relay1: Battery
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2: Solar
  delay(100);
  digitalWrite(Relay3, LOW);                      // Open Relay3: Charge
  delay(100);

  digitalWrite(Relay1, HIGH);                     // Close Relay1: Battery
  delay(100);
  powerADC = analogRead(Bat_Read);                // read the input on analog pin 0 (D14)
  voltage1 = powerADC * (5.0 / 1023.0);           // Convert analog (0 - 1023) to voltage (0 - 5V)
  digitalWrite(Relay1, LOW);                      // Close Relay1
  delay(100);

  Bat_Volt = voltage1 * 3.36;                     // 5V x 3.36 = 16V8 - Convert to actual voltage
  Serial.print("Battery1: ");
  Serial.println(voltage1);
  Serial.print("Battery2: ");
  Serial.println(Bat_Volt);
}

/** READ SOLAR **/
void adc_02_read(void) {
  voltage2 = 0.00;
  Sol_Volt = 0.00;

  digitalWrite(Relay1, LOW);                      // Open Relay1: Battery
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2: Solar
  delay(100);
  digitalWrite(Relay3, LOW);                      // Open Relay3: Charge
  delay(100);

  digitalWrite(Relay2, HIGH);                     // Close Relay1: Battery
  delay(100);
  powerADC = analogRead(Sol_Read);                // read the input on analog pin 0 (D15)
  voltage2 = powerADC * (5.0 / 1023.0);           // Convert analog (0 - 1023) to voltage (0 - 5V)
  digitalWrite(Relay2, LOW);                      // Close Relay1
  delay(100);

  Sol_Volt = voltage2 * 3.36;                  // 5V x 4.27 = 21V35 - Convert to actual voltage
  Serial.print("Solar1: ");
  Serial.println(voltage2);
  Serial.print("Solar2: ");
  Serial.println(Sol_Volt);
}
