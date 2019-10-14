/*
Programming Instruction:
------------------------
* 5V supply switch on programmer
* PA0 is pulled to ground (jumper)
* Permissions:  $ sudo chmod a+rw /dev/ttyUSB0
* External 5V power on board
* Then remove jumper and restart power
* Tools -> Serial_Monitor -> 115200
* IP Address: 192.168.x.x
*/

#include <stdio.h>
#include <WiFi.h>

#include "mainPage.h"

// Replace with your network credentials
const char* ssid     = "Pretty_Fly_For_A_WiFi";
const char* password = "#Mexicana411";
//const char* ssid     = "ESP32MY";
//const char* password = "12345678";

WiFiServer server(80);

/* Output Pins */
const int led1 = 2;       // the number of the LED pin
const int led2 = 17;      // the number of the LED pin

/* Variables Updated to HTML page */
float     val1 = 0.5;
int       val2 = 5;
int       val3 = 110;
int       val4 = 55;

// Client variables 
char linebuf[80];
int charcount=0;
//=======================================================================
// 
//=======================================================================
void setup() {
  // initialize the LEDs pins as an output:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while(!Serial) {
    Serial.print("."); // wait for serial port to connect. Needed for native USB port only
  }
  
  // We start by connecting to a WiFi network
  Serial.println();
  
  //WiFi.begin(ssid, password);
  WiFi.softAP(ssid, password); //Added for soft AP****************
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());//Added for soft AP****************
  
  server.begin();
}
//=======================================================================
// 
//=======================================================================
void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();

          // Main Heading
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>* TSE WIFI Server *</h1>");

          //Table heading
          client.println("<table><tr><th colspan=2>SMART DEVICE<br></th></tr></table>");

          //Table Entry1 and Value 1
          client.println("<table><tr><th>VALUE1: </th><th>VALUE2: </th><th>VALUE3: </th><th>VALUE4: </th></tr>");

          client.println("<tr><td>");
          client.println(val1);
          client.println("</td><td>");
          
          client.println(val1);
          client.println("</td><td>");

          client.println(val1);
          client.println("</td><td>");

          client.println(val1);
          client.println("</td></tr></table>");

          //LED control
          client.println("<p>LED #1 <a href=\"on1\"><button>ON</button></a>&nbsp;<a href=\"off1\"><button>OFF</button></a></p>");
          client.println("<p>LED #2 <a href=\"on2\"><button>ON</button></a>&nbsp;<a href=\"off2\"><button>OFF</button></a></p>");
          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /on1") > 0){
            Serial.println("LED 1 ON");
            digitalWrite(led1, HIGH);
            val1 += 1;
            val3 -= 10;
          }
          else if (strstr(linebuf,"GET /off1") > 0){
            Serial.println("LED 1 OFF");
            digitalWrite(led1, LOW);
          }
          else if (strstr(linebuf,"GET /on2") > 0){
            Serial.println("LED 2 ON");
            digitalWrite(led2, HIGH);
          }
          else if (strstr(linebuf,"GET /off2") > 0){
            Serial.println("LED 2 OFF");
            digitalWrite(led2, LOW);
          }
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}


