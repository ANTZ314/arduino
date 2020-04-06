/* 
 *  OLED [32x64]??
 *  ESP32 - I2C - OLED-SSD1306
 *  SDA - D21 - D0 (pin23)
 *  SCL - D22 - D4 (pin24)
 *  
 *  display.setFont(ArialMT_Plain_10);  <- 10, 16, 24
 *  display.drawString(x, y1, "STRING1");
 *  display.drawString(x, y2, "STRING2");
 *  display.display();
 */
#include <Wire.h>
#include "SSD1306.h" 

//Analog Input
#define ANALOG_PIN_0 36                     // GPIO 36 => ADC1_CH0
int analog_value = 0;

const int MTCK = 13;                        // GPIO LED
//SSD1306  display(0x3c, 21, 22);           // ESP32-Dev-Kit
SSD1306  display(0x3c, 0, 4);               // Monichron Board
 
void setup() {
  Serial.begin(115200);
  delay(1000);                              // Time to bring up serial monitor
  Serial.println("ESP32 Analog IN Test");
  
  pinMode (MTCK, OUTPUT);                   // Initialise GPIO LED
  
  /* Initialise OLED * /
  display.init();
  display.setFont(ArialMT_Plain_10);
  display.drawString(10, 5, "TEMPERATURE");
  display.setFont(ArialMT_Plain_16);
  display.drawString(20, 20, "TEST 1");
  display.display();
  */
}
 
void loop() 
{
  /* Read Analog value & print to serial monitor */
  
  //int sensorValue = analogRead(A0);       // read input analog ADC1_0
  analog_value = analogRead(ANALOG_PIN_0);  // read input analog ADC1_0
  Serial.println(analog_value);             // print

  /* Blink LED as delay */
  digitalWrite (MTCK, HIGH);                // turn on the LED
  delay(500);                               // wait 1sec
  digitalWrite (MTCK, LOW);                 // turn on the LED
  delay(500);                               // wait 500ms
}