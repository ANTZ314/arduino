/*
This program will put the ATMEGA in a sleep mode 
and should wake it up via a hardware Interrupt, INT0 (digital_2)
  ---> Pin2 Wake-Up WORKING...
*/
#define INT_PIN 2

#include <avr/sleep.h>

void setup() 
{
  Serial.begin(9600);                               //Start Serial Comunication
  pinMode(LED_BUILTIN, OUTPUT);                     // LED on pin 13
  pinMode(INT_PIN, INPUT_PULLUP);                   // D2 as interupt (pulled-down)
  Serial.println("Init done");
}

void loop() 
{
  //delay(5000);                                    // needed, otherwise gets stuck in sleep mode? 
  blink();
  Sleep_CPU();                                      // Call function to sleep CPU
}

void Sleep_CPU(void)
{
  Serial.flush();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);              // Sets the sleep mode
  cli();
  sleep_enable();                                   // set sleep bit
  attachInterrupt(digitalPinToInterrupt(INT_PIN), Wake_CPU, RISING); // attach interrupt to wake CPU after sleep. 
  digitalWrite(LED_BUILTIN, LOW);                   // turn off LED to show sleep mode
  sei();
  sleep_cpu();                                      // put to sleep - will wake up here. 
  sleep_disable();
  Serial.println("woke up");                        // first line of code executed after sleep. 
  digitalWrite(LED_BUILTIN, HIGH);                  //turning LED on
}

void Wake_CPU(void)
{
  detachInterrupt(digitalPinToInterrupt(INT_PIN));  //Removes the interrupt from pin 2;
}

/** REMOVE **/
void blink(void){
  digitalWrite(LED_BUILTIN, HIGH);                   // LED on
  delay(200);                                   // wait for a second
  digitalWrite(LED_BUILTIN, LOW);                    // LED off
  delay(200);                                   // wait for a second
}