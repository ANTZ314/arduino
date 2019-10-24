/* 
 *  Monichron Accelerometertest Code - V01 Not Works
 *  ESP32 - I2C - ADXL345 ACCELEROMETER
 *  SDA - D21 (pin42)
 *  SCL - D22 (pin39)
 */
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
  
 
Adafruit_ADXL345_Unified accel;
 
void setup() {
   
  /*Initialize an instance of Adafruit_ADXL345_Unified with a unique id*/
  accel = Adafruit_ADXL345_Unified(12345);
 
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Small example to read from ADXL345 accelerometer");
   
}
 
void loop() {
   
  /*Read from ADXL345 accelerometer*/
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
  Serial.println("m/s^2 ");
  /*Take a one second break*/
  delay(1000);
 
}