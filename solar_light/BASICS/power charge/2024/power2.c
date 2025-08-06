/*
Check the power of the Solar Panel Relay Input (A2)
*/
#include <Wire.h>

/* GPIOs & ADC */
const int Relay2 = 4;                           // Solar Panel Relay (ADC)
const int Sol_Read = A2;                        // Solar analog read

/* Global variables */
float powerADC;                                 // Both power voltage
float voltage2;                                 // converted voltages
float Sol_Volt  = 0.00;                         // Solar Panel voltage
bool once = true;

/**/
void setup() {
  Serial.begin(9600);
  pinMode(Relay1, OUTPUT);                      // init Relay 1
  pinMode(Relay2, OUTPUT);                      // init Relay 2
  pinMode(Relay3, OUTPUT);                      // init Relay 3
  //pinMode(A3, INPUT_PULLUP);

}

void loop() {
  if(once == true){
    relay_on();
    once = false;  
  }

  /* Check Solar Voltage */
  adc_02_read();                                // Read Solar voltage
  delay(3000);

  //relay_off();

}

void relay_on(void){
  digitalWrite(Relay1, LOW);                      // Open Relay1: Battery
  delay(100);
  digitalWrite(Relay2, LOW);                      // Open Relay2: Solar
  delay(100);
  digitalWrite(Relay3, LOW);                      // Open Relay3: Charge
  delay(100);

  digitalWrite(Relay2, HIGH);                     // Close Relay1: Battery
  delay(100);
}

/** READ SOLAR **/
void adc_02_read(void) {
  voltage2 = 0.00;
  Sol_Volt = 0.00;

  powerADC = analogRead(Sol_Read);                // read the input on analog pin 0 (D15)
  voltage2 = powerADC * (5.0 / 1023.0);           // Convert analog (0 - 1023) to voltage (0 - 5V)

  Sol_Volt = voltage2 * 3.36;                     // 5V x 4.27 = 21V35 - Convert to actual voltage
  Serial.print("Solar1: ");
  Serial.println(voltage2);
  //Serial.print("Solar2: ");
  //Serial.println(Sol_Volt);
}

void relay_off(void){
  digitalWrite(Relay2, LOW);                      // Close Relay1
  delay(100);
}
