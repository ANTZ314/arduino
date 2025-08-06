/*
  Nano Every: Cycle of Blink 5 times then Sleep for 8 seconds
  --> WORKS: Wake-up every ~6 sec
  --> On 5V input, ~30mA in sleep??
  --> Including OLED splash display
*/

#include <avr/sleep.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

uint8_t loopCount = 0 ;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void RTC_init(void) {
  while (RTC.STATUS > 0) ;                 // Wait for all register to be synchronized
 
  RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;        // Run low power oscillator (OSCULP32K) at 1024Hz for long term sleep
  RTC.PITINTCTRL = RTC_PI_bm;              // PIT Interrupt: enabled
  /* Set period 8 seconds (see data sheet) and enable PIC */
  RTC.PITCTRLA = RTC_PERIOD_CYC8192_gc | RTC_PITEN_bm;
}

ISR(RTC_PIT_vect) {
  RTC.PITINTFLAGS = RTC_PI_bm;          // Clear interrupt flag by writing '1' (required) 
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  RTC_init();   
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Set sleep mode to POWER DOWN mode 
  sleep_enable();                       // Enable sleep mode, but not going to sleep yet

  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));           // (REMOVE) 
    for(;;);                                    // Don't proceed, loop forever
  }
  // Show initial display buffer
  display.display();                            // Adafruit Welcome Logo (REMOVE/CHANGE)
  delay(2000);                                  // Pause for 2 seconds
  Serial.println("Setup Complete");
}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on (HIGH is the voltage level)
  delay(200);                         // wait for a second
  digitalWrite(LED_BUILTIN, LOW);     // turn the LED off by making the voltage LOW
  delay(200);                         // wait for a second

  if(loopCount == 4) {
    Serial.print("Sleep... ");
  }
  if( ++ loopCount == 5 ) {
    //Serial.println("-");
    loopCount = 0 ;
    sleep_cpu();                      // Sleep the device and wait for an interrupt to continue
  }
  if(loopCount == 0) {
    Serial.print("Awake ");
  }
}
