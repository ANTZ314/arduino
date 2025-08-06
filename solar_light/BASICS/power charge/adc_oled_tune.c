/*
PARTS: OLED + Relay + ADC Read + Push_Button:
Button Press -> Relay Close -> Read ADC -> Display Voltage
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

const int Vbat = 3;                             // Relay: Vbat
const int Vsol = 2;                             // Relay: Vsol
//const int Vled = 5;                           // Relay: Vled
const int buttonPin = 11;                       // Pushbutton pin
const int ledPin = 13;                          // BUilt-in LED pin

int sensorValue;
float voltage;
float Bat_Volt = 0.00;                          // printed voltage
int buttonState = 0;                            // pushbutton status


/* Startup Routine */
void setup() {
  Serial.begin(9600);
  //pinMode(Vbat, OUTPUT);                        // Relay: Battery
  pinMode(Vsol, OUTPUT);                        // Relay: Solar 
  //pinMode(Vled, OUTPUT);                      // Relay: LED strip
  pinMode(ledPin, OUTPUT);                      // initialize LED
  pinMode(buttonPin, INPUT);                    // initialize pushbutton

  /* SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally */
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));           // (REMOVE) 
    for(;;);                                    // Don't proceed, loop forever
  }
  // Show initial display buffer
  display.display();                            // (REMOVE)
  delay(2000);                                  // Pause for 2 seconds
  display.clearDisplay();                       // Clear the buffer
}

void loop() {
  
  buttonState = digitalRead(buttonPin);         // read pushbutton value

  /* Button pressed? */
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);                 // turn LED on
    Serial.println("PRESSED");                  // REMOVE
    delay(500);                                 // Debounce press
    /* Read volatge and display */
    //digitalWrite(Vbat, HIGH);                   // Close Relay1
    digitalWrite(Vsol, HIGH);                   // Close Relay2
    adc_read();                                 // Read Battery voltage
    testdrawstyles();                           // Display on OLED
  } else {
    digitalWrite(ledPin, LOW);                  // turn LED off
    //digitalWrite(Vbat, LOW);                    // Open Relay1
    digitalWrite(Vsol, LOW);                    // Open Relay2
  }
}

void adc_read(void) {
  //sensorValue = analogRead(A1);                 // read Vbat [16V8 = 5V]
  sensorValue = analogRead(A2);                 // read Vsol [16V8 = 5V]
  //sensorValue = analogRead(A3);                 // read Vldr [16V8 = 5V]
  voltage = sensorValue * (5.0 / 1023.0);       // Convert analog (0 - 1023) to voltage (0 - 5V)
  Serial.println(voltage);                      // print out the value you read

  /* Convert to 16V8 range */
  Bat_Volt = voltage;                           // int = float
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);                       // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);          // Draw white text
  display.setCursor(0,0);                       // Start at top-left corner
  display.println(F("Power Monitor (V01):"));

  display.setTextColor(SSD1306_WHITE);          // Draw white text
  display.println("------------------");

  display.setTextSize(2);                       // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

   display.print(Bat_Volt); display.println(F("V"));

  display.display();
  delay(2000);                                  // Display voltage for 2sec
  display.clearDisplay();

}