/*
Programming Instruction:
------------------------
* 5V supply switch on programmer
* PA0 is pulled to ground (jumper)
* Permissions:  $ sudo chmod a+rw /dev/ttyUSB0
* External 5V power on board
* Then remove jumper and restart power
*/

#include <stdio.h>
#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "ESP32MY";
const char* password = "12345678";

WiFiServer server(80);

const int led1 =  4;      // the number of the LED pin
const int led2 =  17;      // the number of the LED pin
float Usb1 = 0.5;
int Usb2 = 1;
int plug1 = 4;
int plug2 = 6;

// Client variables 
char linebuf[80];
int charcount=0;

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
  //Serial.println();
  Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  
  //WiFi.begin(ssid, password);
  WiFi.softAP(ssid, password); //Added for soft AP****************

  //?????????????????????????????????????
  // attempt to connect to Wifi network:
  //while(WiFi.status() != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  //  delay(500);
  //  Serial.print(".");
  //
  //?????????????????????????????????????
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());//Added for soft AP****************
  
  server.begin();
}

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
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>MUWS - WebServer</h1>");
          
          client.println("<table><tr><th colspan=2>SMART DEVICE<br></th></tr></table>");//Table heading
//Table Entry1 and Value 1
          client.println("<table><tr><th>USB1</th><th>");
          client.println(Usb1);
          client.println("A</th></tr></table>");

//Table Entry2 and Value 2
          client.println("<table><tr><th>USB2</th><th>");
          client.println(Usb2);
          client.println("A</th></tr></table>");

//Table Entry3 and Value 3
          client.println("<table><tr><th>PLUG1</th><th>");
          client.println(plug1);
          client.println("A</th></tr></table>");

//Table Entry4 and Value 4
          client.println("<table><tr><th>PLUG2</th><th>");
          client.println(plug2);
          client.println("A</th></tr></table>");

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


