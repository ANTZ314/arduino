
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

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led1, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness:
  analogWrite(led1, brightness1);
  analogWrite(led2, brightness2);
  analogWrite(led3, brightness3);

  // Increment thru all 7 R-G-B combinations
  if(Flag <= 0x07){         
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
  for(i=0; i<3; i++){                                 // check R-G-B
      temp = Flag;                                    // copy Flag
      temp = temp && mask;                            // check for high
      if(temp > 0x00 && mask == 0x01){led_R = 1;}     // Led1 goes high
      else{led_R = 0;}
      if(temp > 0x00 && mask == 0x02){led_G = 1;}     // Led2 goes high
      else{led_G = 0;} 
      if(temp > 0x00 && mask == 0x04){led_B = 1;}     // Led3 goes high
      else{led_B = 0;}
      mask = mask * 2;                                // double the mask (shift left)
  }
  
  
  // change the brightness for next time through the loop:
  brightness1 = brightness1 + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness1 <= 0 || brightness1 >= 255) {
    fadeAmount = -fadeAmount;                     // fadeAmount = -5
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}