/*
  Sketch name:  Slave Blink - WiFi
  UNED Arduino IoT lab.

  A simple web server acting as a Slave that turns, under commands from client 
  'Master Blink - WiFi' example, a LED on and off through GET requests.

  This sketch will connect to an access point created by sketch 'Master Blink - WiFi'.
  
  This example is written for a network using WPA encryption. For
  WEP or WPA, change the WiFi.begin() call accordingly.

  Circuit:
    - RGB led connected to pins A0 (red), A1 (green) and A2 (blue)
*/

#include <WiFiNINA.h>

#define RGB_PIN A0          

// Details for Wifi network access point created by: Master Blink - WiFi
char ssid[] = "WifiNINA-Access-Point";  // your network SSID (name)
char pass[] = "123456789";              // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                       // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);         // server on port 80

void setup() {
  pinMode(RGB_PIN, OUTPUT);               // initialize digital pin RGB_PIN as an output.
  digitalWrite(RGB_PIN, LOW);             // start with the LED off

  WiFi.config(IPAddress(192, 168, 4, 2)); // set IP address
  while (status != WL_CONNECTED) {        // attempt to connect to Wifi network
    status = WiFi.begin(ssid, pass);      // connect to WPA/WPA2 network created by: Wifi_Temp_Control_-_Sensor
    delay(1000);                          // wait 1 seconds before next connection attempt
  }

  server.begin();                     // start the web server on port 80
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    String currentLine = "";                // make a String to hold incoming data from the client
    boolean currentLineIsBlank = true;      // boolean to confirm if current Line sent by client Is Blank    
    while (client.connected()) {            // loop while the client's connected      
      if (client.available()) {             // if there's new bytes to read from the client,
        char c = client.read();             // read a byte
        // if the byte is a newline character (\n)
        // then you've gotten to the end of the line
        // and if you got two newline characters in a row (\n) it means that
        // the http request has ended, so you could send a reply.
        if (c == '\n' && currentLine.length() == 0) {
          ;// no reply required 
        }
        if (c == '\n') {              // if you got a newline, then clear currentLine:
          currentLine = "";
        } else if (c != '\r') {       // if you got anything else but a carriage return character,
          currentLine += c;           // add it to the end of the currentLine
        } 
        
        if (currentLine.endsWith("GET /H")){         // client request to turn led on
          digitalWrite(RGB_PIN, HIGH);               // turn the LED on 
        } else if (currentLine.endsWith("GET /L")){  // client request to turn led off
          digitalWrite(RGB_PIN, LOW);                // turn the LED off 
        }
      }
    }
    delay(1);                                 // give client time to receive the data

    client.stop();                            // close the connection:
  }

  if (WiFi.status() != WL_CONNECTED) {          // confirm still connected to network
    setup();                                    // otherwise rerun setup
  }  
  
}
