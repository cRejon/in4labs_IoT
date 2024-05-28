/*
  Sketch name: Temp Control Fan - WiFi
  UNED Arduino IoT lab. 

  WiFi Temperature and Humidity control - Fan

  A simple web server that allows clients to turn on a fan.
  This sketch will connect to an access point created by sketch: Temp Control LCD - WiFi
  
  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  Circuit:
    - Fan (+) conected to 12 V (+)
    - Fan (-) conected to Relay NC (Normally Closed) 
    - Relay COM (common) to ground
    - Pin 2 connected to Relay Signal pin
 
  Review example 'Fan Test' for a detailed description of how this fan works.
*/

#include <WiFiNINA.h>

#define FAN_PIN 2             // define the pin connected to Relay

// Details for Wifi network access point:
char ssid[] = "WifiNINA-Access-Point";  // your network SSID (name)
char pass[] = "123456789";              // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                       // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);         // server on port 80

void setup() {
  WiFi.config(IPAddress(192, 168, 4, 3));
  while (status != WL_CONNECTED) {        // attempt to connect to Wifi network
    status = WiFi.begin(ssid, pass);      // connect to WPA/WPA2 network created by: Temp_Control_Sensor_-_Wifi
    delay(1000);                          // wait 1 seconds before next connection attempt
  } 

  server.begin();                         // start the web server on port 80

  pinMode(FAN_PIN, OUTPUT);               // set the FAN pin as an output
  digitalWrite(FAN_PIN, LOW);            // fan off
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
          break;  // no reply required 
        }
        if (c == '\n') {                // if you got a newline, then clear currentLine:
          currentLine = "";
        } else if (c != '\r') {         // if you got anything else but a carriage return character,
          currentLine += c;             // add it to the end of the currentLine
        } 
        
        if (currentLine.endsWith("GET /H")){        // client request to turn fan on
          digitalWrite(FAN_PIN, HIGH);               // turn the FAN on
        } else if (currentLine.endsWith("GET /L")){ // client request to turn fan off
          digitalWrite(FAN_PIN, LOW);              // turn the FAN off
        }
      }
    }
    delay(1);                           // give client time to receive the data

    client.stop();                      // close the connection:
  }

  // check if WiFi is still connected, if not, reconnect
  if (WiFi.status() != WL_CONNECTED) {
    setup();
  }
}
