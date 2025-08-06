/*
OLED screen sleep test code:
  -> Button press (D2) initiates oled screen
  -> Dummy data: 16.8V, 20.3V, Charge  --> Sleep
  -> Also serial prints each step.

Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Arduino UNO:         A4(SDA),  A5(SCL)
Arduino Nano Every:  D18(SDA), D19(SCL)
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels


#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* GPIOs & ADC */
const int buttonPin = 2;                        // Pushbutton pin
const int ledPin = 13;                          // BUilt-in LED pin <--- REMOVE

/**/
float Act_Bat_V = 16.80;                        // Actual voltage (converted)
float Act_Sol_V = 20.35;                        // Actual voltage (converted)
int buttonState = 0;                            // pushbutton status


void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);                    // init pushbutton
  pinMode(ledPin, OUTPUT);                      // init Onboard LED

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();                            // Show initial display buffer
  delay(2000);                                  // Pause for 2 seconds
  display.clearDisplay();                       // Clear the buffer

}

void loop() {
  user_check();                                 // Check button & display voltages

}

/** User Status Check **/
void user_check(void) {
  buttonState = digitalRead(buttonPin);         // read pushbutton value

  /* Button pressed */
  if (buttonState == HIGH) {
    Serial.println("BUTTON!");                   // REMOVE!

    oled_display(0);                            // Display Battery Voltage
    delay(1500);                                // 1.5 sec
    oled_display(1);                            // Display Solar Voltage
    delay(1500);                                // 1.5 sec
    oled_display(2);                            // Display Solar Voltage
    delay(1500);                                // 1.5 sec
    oled_display(3);                            // Display Solar Voltage
    
    // Display off??
    delay(1000);
    digitalWrite(ledPin, LOW);                  // turn LED off
  }
}

/** Display current volatge levels **/
void oled_display(int bat_sol) {
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();

  display.setTextSize(1);                       // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);          // Draw white text
  display.setCursor(0,0);                       // Start at top-left corner
  if(bat_sol == 0){
    display.println(F("Battery Voltage:"));
  }
  else if(bat_sol == 1){
    display.println(F("Solar Voltage:"));
  }
  else if(bat_sol == 2){
    display.println(F("Control Mode:"));
  }

  display.setTextColor(SSD1306_WHITE);          // Draw white text
  display.println("------------------");

  display.setTextSize(2);                       // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  if(bat_sol == 0){
    display.print(Act_Bat_V);                   // 16.8V / 3.36 = 5V
    display.println(F("V"));
    
    Serial.print("Battery2: ");                   // REMOVE!
    Serial.println(Act_Bat_V);                    // REMOVE!
  }
  else if(bat_sol == 1){
    display.print(Act_Sol_V);                   // 21.4V / 4.28 = 5V
    display.println(F("V"));
    
    Serial.print("Solar2: ");                     // REMOVE!
    Serial.println(Act_Sol_V);                    // REMOVE!
  }
  else if(bat_sol == 2){
    Serial.println("Charge...");                     // REMOVE!
    display.println(F("Charging"));
  }

  display.display();
  delay(2000);                                  // Display voltage for 2sec
  display.clearDisplay();
  
  if(bat_sol == 3){
    Serial.println("Screen Off");                     // REMOVE!
    //bat_sol = 0;                                // no need to clear here
    display.ssd1306_command(SSD1306_DISPLAYOFF);  // <---- TEST
  }
}