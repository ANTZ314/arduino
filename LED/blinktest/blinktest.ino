int led1 = 9;         // First PWM pin
int led2 = 6;         // Second PWM pin
int led3 = 5;         // Third PWM pin
int led4 = 4;         // Third PWM pin

// the setup routine runs once when you press reset:
void setup() {
  pinMode(led1, OUTPUT);                  // declare pin 9 & 10 to be outputs:
  pinMode(led2, OUTPUT);                  // declare pin 9 & 10 to be outputs:
  pinMode(led3, OUTPUT);                  // declare pin 11 to be an output:
  pinMode(led4, OUTPUT);                  // declare pin 11 to be an output:
  
}

void loop() {
    digitalWrite(led1, HIGH);
    delay(300);
    digitalWrite(led1, LOW);
    delay(300);
    digitalWrite(led2, HIGH);
    delay(300);
    digitalWrite(led2, LOW);
    delay(300);
    digitalWrite(led3, HIGH);
    delay(300);
    digitalWrite(led3, LOW);
    delay(300);
    digitalWrite(led4, HIGH);
    delay(300);
    digitalWrite(led4, LOW);
    delay(300);
}
