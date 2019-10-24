/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects (Some boards use this port for SPI Flash access)
 * U2UXD is unused and can be used for your projects.
 * 
 * Power-Up Requirements:
 * VBAT     - High
 * BG96_EN  - High
 * RESET_N  - High
 * PWRKEY   - High [30ms] - Low [>=100ms] - High
 * [After 4.8 sec] STATUS = High
*/
//#include <HardwareSerial.h>
/* GSM Serial Pins */
#define RXD2 16       // PIN_25
#define TXD2 17       // PIN_27

/* GSM Control Pins */
#define BG96_EN 35    // PIN_11 - Load Switch
#define PWR_KEY 2     // PIN_22
#define RESET_N 26    // PIN_15

#define GPIO1 13      // Indicator LED

String AT;
String response = "OK";            // output from string functions placed here

void setup() {
  /* Setup both serial comms */
  Serial.begin(9600);  //115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);     // Serial port 2

  /* Output Control Pins */
  pinMode(BG96_EN, OUTPUT);
  pinMode(PWR_KEY, OUTPUT);
  pinMode(RESET_N, OUTPUT);
  pinMode(GPIO1, OUTPUT);
  
  Serial.println("Serial started...");
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));
  delay(500);
}
 
void loop() {
  int count = 0;
  
  /* Send request command */
  Serial2.println("ATI");
  
  /* If Tx looped to Rx? - print what is sent */
  while (Serial2.available())
  {
    if(count == 10){ 
      Serial2.println("ATI");             // send test AT command
      count = 0;                          // clear counter
    } // ATI = Product ID
    
    Serial.print(char(Serial2.read()));   // print response
    count++;                              // increment counter
    delay(100);
  }
  blink_led();
  
}

/* Power Up Sequence for the BG96 GSM/GPS Module */
void BG96_PWR_UP() {
  digitalWrite (BG96_EN, HIGH);     // turn on the BG96 Load Switch
  digitalWrite (RESET_N, HIGH);     // Not in Reset Mode
  
  /* Power must be stable >30ms before dipping PWRKEY for >=100ms */
  digitalWrite (RESET_N, LOW);      // Low
  delay(110);                       // wait 110ms
  digitalWrite (RESET_N, HIGH);     // High

}

/* Power Down Sequence for the BG96 GSM/GPS Module */
void BG96_PWR_DOWN() {
  /* Must be held Low for >=650ms */
  digitalWrite (RESET_N, LOW);      // Low
  delay(660);                       // wait 110ms
  digitalWrite (RESET_N, HIGH);     // High
}

void blink_led() {
  digitalWrite (GPIO1, HIGH);       // turn on the LED
  delay(1000);                      // wait 500ms
  digitalWrite (GPIO1, LOW);        // turn on the LED
  delay(500);                       // wait 500ms
}
