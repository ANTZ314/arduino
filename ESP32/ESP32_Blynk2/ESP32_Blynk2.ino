
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

// You should get Auth Token in the Blynk App
char auth[] = "7128dcbc490c46cfbad30654782650a0";

// Your WiFi credentials
char ssid[]       = "Pretty_Fly_For_A_WiFi";
char pass[]       = "#Mexicana411";
char server[]     = "blynk-cloud.com";
unsigned int port = 8442;

const int ledPin = 5;
const int LED_BUILTIN = 2;

void setup()
{
  // Debug console
  Serial.begin(115200);
  
  pinMode (ledPin, OUTPUT);
  pinMode (LED_BUILTIN, OUTPUT);
  

  Blynk.connectWiFi(ssid, pass); // used in place of Blynk.begin(auth, ssid, pass, server, port);

  // line below needs to be BEFORE Blynk.connect()
  timer.setInterval(11000L, CheckConnection); // check if still connected every 11s  
  
  Blynk.config(auth, server, port);
  Blynk.connect(); 
  
}

void CheckConnection(){    // check every 11s if connected to Blynk server
  if(!Blynk.connected()){
    Serial.println("Not connected to Blynk server"); 
    Blynk.connect();  // try to connect to server with default timeout
  }
  else{
    Serial.println("Connected to Blynk server");     
  }
}

void loop()
{
  if(Blynk.connected()){
    Blynk.run();
  }
  else{
    digitalWrite (LED_BUILTIN, HIGH);  // turn on the LED
    digitalWrite (ledPin, HIGH);  // turn on the LED
    delay(500);  // wait for half a second or 500 milliseconds
    digitalWrite (LED_BUILTIN, LOW);  // turn off the LED
    digitalWrite (ledPin, LOW);  // turn off the LED
    delay(100);  // wait for half a second or 500 milliseconds
  }

  
}

