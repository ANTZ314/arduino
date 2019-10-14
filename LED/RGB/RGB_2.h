
int led1 = 3;           // the PWM pin the LED is attached to
int led2 = 5;           // the PWM pin the LED is attached to
int led3 = 6;           // the PWM pin the LED is attached to
int brightness1 = 0;    // how bright the LED is
int brightness2 = 0;    // how bright the LED is
int brightness3 = 0;    // how bright the LED is
int fadeAmount = 5;     // how many points to fade the LED by
char Flag = 0x00;       // count through 7 binary posabilities
char temp = 0x00;       // masking
int i = 0;              // generic counter
char led_R = 0;
char led_G = 0;
char led_B = 0;
char mask = 0x00;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  /*
  // set the brightness:
  analogWrite(led1, brightness1);
  analogWrite(led2, brightness2);
  analogWrite(led3, brightness3);
  */

  /*
  digitalWrite(led1, HIGH);           // it shows not pressed
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  delay(500);  
  // ALL OFF AGAIN
  digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(led2, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(led3, LOW);    // turn the LED off by making the voltage LOW
  delay(500);*/

  
  // Increment thru all 7 R-G-B combinations
  if(Flag < 0x07){         
    Flag = Flag + 0x01;     // increment on each full loop
  }
  else{
    Flag = 0x00;                                      // reset flag
  }
  
  // Clear R-G-B flags before check
  led_R = 0;
  led_G = 0;
  led_B = 0;
  mask  = 0x01;                                       // reset mask
  
  // Check which should be high
  temp = Flag;                                    // copy Flag
  
  temp = temp & mask;                            // check for high
  if(temp > 0x00 && mask == 0x01){led_R = 1;}     // Led1 goes high
  else{led_R = 0;}
  mask = mask * 2;                                // double the mask (shift left)
  
  temp = Flag;                                    // copy Flag
  temp = temp & mask;                            // check for 
  if(temp > 0x00 && mask == 0x02){led_G = 1;}     // Led2 goes high
  else{led_G = 0;} 
  mask = mask * 2;                                // double the mask (shift left)
      
  temp = Flag;                                    // copy Flag
  temp = temp & mask;                            // check for 
  if(temp > 0x00 && mask == 0x04){led_B = 1;}     // Led3 goes high
  else{led_B = 0;}

  //led_R = 0;
  if (led_R == 1){              // it shows pressed
    digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  else if(led_R == 0){    
    digitalWrite(led1, HIGH);    // turn the LED off by making the voltage LOW
  }
  if (led_G == 1){            // it shows not pressed
    digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  else if(led_G == 0){
     digitalWrite(led2, HIGH);    // turn the LED off by making the voltage LOW
  }
  if (led_B == 1){            // it shows not pressed
    digitalWrite(led3, LOW);    // turn the LED off by making the voltage LOW
  }
  else if(led_B == 0){
     digitalWrite(led3, HIGH);    // turn the LED off by making the voltage LOW
  }
  delay(500);  
  // ALL OFF AGAIN
  digitalWrite(led1, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(led2, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(led3, HIGH);    // turn the LED off by making the voltage LOW
  delay(500);
  
  /*
  // change the brightness for next time through the loop:
  brightness1 = brightness1 + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness1 <= 0 || brightness1 >= 255) {
    fadeAmount = -fadeAmount;                     // fadeAmount = -5
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
  */
}