/*
Custom OLED + ADC Read
Displays the ADC Voltage level every 1 second
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Arduino UNO:         A4(SDA),  A5(SCL)
// Arduino Nano Every:  D18(SDA), D19(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float Bat_Volt = 0.00;

/**/
void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer
  display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();

  testdrawstyles();      // Draw 'stylized' characters

}

void loop() {
  int sensorValue = analogRead(A0);             // read the input on analog pin 0
  float voltage = sensorValue * (5.0 / 1023.0); // Convert analog (0 - 1023) to voltage (0 - 5V)
  Serial.println(voltage);                      // print out the value you read

  Bat_Volt = voltage;                           // int = float
  testdrawstyles();                             // Draw 'stylized' characters
  delay(1000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);                     // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);                     // Start at top-left corner
  display.println(F("Power Monitor (V01):"));

  //display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  //display.println(3.141592);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.println("------------------");

  display.setTextSize(2);                     // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

   display.print(Bat_Volt); display.println(F("V"));

  display.display();
}
