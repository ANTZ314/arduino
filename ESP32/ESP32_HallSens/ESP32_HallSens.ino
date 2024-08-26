/***
 * Hall Effect Sensor output to serial monitor [115200 baud]
 * Code Written by TechMartian
 */
int measurement = 0;
const int LED_BUILTIN = 2;

 void setup() {
  Serial.begin(115200);
  pinMode (LED_BUILTIN, OUTPUT);
  
}

void loop() 
{
  measurement = hallRead();
  Serial.print("Hall sensor measurement: ");
  Serial.println(measurement);
  
  digitalWrite (LED_BUILTIN, HIGH);   // turn on the LED
  delay(500);                         // wait
  digitalWrite (LED_BUILTIN, LOW);    // turn off the LED
  delay(500);                         // wait
}
