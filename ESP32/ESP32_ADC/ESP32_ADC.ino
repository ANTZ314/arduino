/***
 * ADC reading output to serial monitor
 * Code Written by TechMartian
 */
#include <driver/adc.h>
 
int measurement = 0;
const int LED_BUILTIN = 2;
int val = 0;

 void setup() {
  Serial.begin(115200);
  pinMode (LED_BUILTIN, OUTPUT);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
  
}

void loop() 
{
  val = adc1_get_raw(ADC1_CHANNEL_0);
  Serial.print("ADC measurement: ");
  Serial.println(val);
  
  digitalWrite (LED_BUILTIN, HIGH);   // turn on the LED
  delay(500);                         // wait
  digitalWrite (LED_BUILTIN, LOW);    // turn off the LED
  delay(500);                         // wait
}
