/*libraries needed
Search esp8266 and esp32 oled driver in library manager
search max30105 in library manager
*/
#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"
#include "SSD1306Wire.h"  // legacy include: `#include "SSD1306.h"`

#define sda 0             // OLED_SDA     - Banish 23
#define scl 4             // OLED_SCL     - Banish 24

#define I2ConeSDA 21      // HR I2C SDA   - Banish 21
#define I2ConeSCL 22      // HR I2C SCL   - Banish 22

MAX30105 particleSensor;
SSD1306Wire  display(0x3c, sda, scl);

const int LED = 13;       // LED GPIO

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];    //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;        //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
TwoWire I2Cone = TwoWire(1);



void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");

  pinMode (LED, OUTPUT);
  
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.drawString(10, 5, "HEART RATE:");
  display.setFont(ArialMT_Plain_16);
  display.drawString(15, 20, "--- BPM");
  display.display();

  
  //I2Cone.begin(I2ConeSDA, I2ConeSCL, 400000);
  I2Cone.begin(21, 22, 400000);

  
  // Initialize sensor
  if (!particleSensor.begin(I2Cone, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
    {
      /* Blink LED if Heart Rate Sensor Error */
      digitalWrite (LED, HIGH);     // turn on the LED
      delay(1000);                  // wait 500ms
      digitalWrite (LED, LOW);      // turn on the LED
      delay(500);                   // wait 500ms
    }
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup();                     //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);  //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);   //Turn off Green LED
}

void loop()
{
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
  
  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  if (irValue < 50000)
  {
    Serial.print(" No finger?");
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 22, " No finger?");
    display.display();
    Serial.println();
  }
  if (irValue > 50000)
  {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 22, " BPM= " + String(beatsPerMinute));
    display.display();
  }
  //}
}
