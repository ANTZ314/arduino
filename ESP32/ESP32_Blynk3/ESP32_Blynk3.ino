
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App
char auth[] = "882ea88b63bb409a8cb262892439deb3";

// Your WiFi credentials
#define ssid "Pretty_Fly_For_A_WiFi"
#define pass "#Mexicana411"

BlynkTimer timer;

const int LED = 2;
const int BTN = 4;
int cunt;
int dick;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more than 10 values per second.
  //Blynk.virtualWrite(V0, millis() / 1000);    // Update each second counter to V0

  dick = digitalRead(BTN);
  if(dick == 1)
  {
    Blynk.virtualWrite(V0, "Okay it's about  ");
    Blynk.virtualWrite(V1, "Time to give up  ");
  }
  else  {
    Blynk.virtualWrite(V0, "  This is a new");
    Blynk.virtualWrite(V1, "---WORLD ORDER---");
  }
  
  if(cunt == 0)
  {
    cunt = 1;   // Toggle
    digitalWrite(LED, HIGH);
    
  }
  else{
    cunt = 0;   // Toggle
    digitalWrite(LED, LOW);
    
  }
  
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BTN, INPUT);
  cunt = 0;
  dick = 0;

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  
}


