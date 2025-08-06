/*
Custom OLED + ADC Read (x2) + Push_Button:
  -> Continuously check ADC_01 & ADC_02 for under/over voltage
    => If bat(<16v8) && solar(>16V8) --> Charge
    => If bat(>14v8) && light(night) --> LEDs On
  -> Button Press:
    => Displays the Battery Voltage Level for 2 seconds
  -> [LATER] Mode Selection:
    => Mode_01: Long slow blink (breathing?)
    => Mode_02: Short Fast blink
    => Mode_03: Full On / Full Off
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

const int buttonPin = 10;                       // Pushbutton pin
const int ledPin = 13;                          // BUilt-in LED pin <--- REMOVE
const int ledStrip = 8;                         // BUilt-in LED pin

int sensorValue;
float voltage;
float Bat_Volt = 0.00;                          // Battery voltage
float Solar_Volt = 0.00;                        // Solar Panel voltage
float LDR_Volt = 0.00;                          // LDR Sensor voltage
int buttonState = 0;                            // pushbutton status


/**/
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);                      // init Onboard LED
  pinMode(buttonPin, INPUT);                    // init pushbutton
  pinMode(ledStrip, OUTPUT);                    // init LED Strip
  // No need to define ADC pins as inputs?

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));           // (REMOVE) 
    for(;;);                                    // Don't proceed, loop forever
  }
  // Show initial display buffer
  display.display();                            // (REMOVE)
  delay(2000);                                  // Pause for 2 seconds
  display.clearDisplay();                       // Clear the buffer
  //testdrawstyles();                             // Draw 'stylized' characters

}

void loop() {
  
  buttonState = digitalRead(buttonPin);         // read pushbutton value

  /* Button pressed? */
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);                 // turn LED on
    Serial.println("PRESSED");                  // REMOVE
    delay(500);                                 // Debounce press
    adc_read();                                 // Read Battery voltage
    testdrawstyles();                           // Display on OLED
  } else {
    digitalWrite(ledPin, LOW);                  // turn LED off
  }
  /* Check Battery Voltage */
  adc_01_read();                                // Read Battery voltage
  delay(50);
  /* Check Solar Voltage */
  adc_02_read();                                // Read Solar voltage
  delay(50);
  /* Check Solar Voltage */
  adc_03_read();                                // Read Solar voltage
  delay(50);

  /* Action_01: */
  Charge_Bat();

  /* Action_02: */
  Aziz_Light();

  /* Mode Select */
  // Later...
}

void adc_01_read(void) {
  sensorValue = analogRead(A0);                 // read the input on analog pin 0 (D14)
  voltage = sensorValue * (5.0 / 1023.0);       // Convert analog (0 - 1023) to voltage (0 - 5V)
  //Serial.println(voltage);                      // print out the value you read

  Bat_Volt = voltage;                           // float = float
}

/** NOTE: Voltage Ratio = 3.333 **/
void adc_02_read(void) {
  sensorValue = analogRead(A1);                 // read the input on analog pin 0 (D15)
  voltage = sensorValue * (5.0 / 1023.0);       // Convert analog (0 - 1023) to voltage (0 - 5V)
  //Serial.println(voltage);                      // print out the value you read

  Solar_Volt = voltage;                         // float = float
}

/** NOTE: Voltage Ratio = 3.333 **/
void adc_03_read(void) {
  sensorValue = analogRead(A1);                 // read the input on analog pin 0 (D15)
  voltage = sensorValue * (5.0 / 1023.0);       // Convert analog (0 - 1023) to voltage (0 - 5V)
  //Serial.println(voltage);                      // print out the value you read

  LDR_Volt = voltage;                           // float = float
}

void Charge_Bat(void) {
  /** NOTE: Battery Voltage Ratio = 3.333 **/
  /*        [16v8 = 5.04V] [14v8 = 4.44V] **/
  
  /* FIND SOLAR VOLTAGE RATIO: 21Vmax -> 16V8 */
  if (Bat_Volt <= 5.03 && Solar_Volt >= 4.5){
    digitalWrite(ledStrip, HIGH);               // LED Strip On
  }
  else{
    digitalWrite(ledStrip, HIGH);               // LED Strip On
  }
}

void Aziz_Light(void) {
  /** CHECK LOW-LIGHT RANGE **/
  if (LDR_Volt <= 100) {
    digitalWrite(ledStrip, HIGH);                 // LED Strip On
    delay(2000);                                  // Delay
    digitalWrite(ledStrip, LOW);                  // LED Strip Off
    delay(1850);                                  // Delay
  }
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
