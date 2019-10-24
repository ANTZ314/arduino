/*
 * SerialOutput sketch
 * Print numbers to the serial port
 * Set Serial Monitor baud rate to 115200 or 9600
*/
void setup()
{
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  //Serial.begin(115200);
}

void loop()
{
    Serial.println("TEST...");
    delay(2000);
}
