/* 
 *  ESP32 - ADC (Monichron GSR Test)
 *  
 *  SENSOR_VP => GPIO 36 => ADC1_CH0  -> Battery
 *  SENSOR_VN => GPIO 39 => ADC1_CH3  -> GSR Sensor
 * 
 */
#include <Wire.h>

/* Analog Input */
//#define ANALOG_PIN_0  36                  // SENSOR_VP => GPIO 36 => ADC1_CH0
#define ANALOG_PIN_3  39                    // SENSOR_VN => GPIO 39 => ADC1_CH3
int analog_value = 0;                       // initialise ADC value

void setup() {
  Serial.begin(115200);
  delay(1000);                              // Time to bring up serial monitor
  Serial.println("ESP32 Analog READ Test:");
}
 
void loop() 
{
  /* Read Analog_0 value */
  //int sensorValue = analogRead(A0);         // read input analog ADC1_0
  //analog_value = analogRead(ANALOG_PIN_0);  // read input analog ADC1_0
  
  /* Read Analog_3 value */
  //int sensorValue = analogRead(A3);         // read input analog ADC1_3
  analog_value = analogRead(ANALOG_PIN_3);    // read input analog ADC1_3
  
  /* Print to serial monitor */
  Serial.println("ADC Output: ");
  Serial.println(analog_value);               // print value
  delay(1000);                                // wait
}