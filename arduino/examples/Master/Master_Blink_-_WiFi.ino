/*
  Sketch name: Master Blink - WiFi
  UNED Arduino IoT lab.

  A web client acting as a Master that turns an LED on, from 'IoT Blink Slave - WiFi'
  example, for a set delay time, then off, repeatedly. 

  This sketch will create a new WiFi access point for the other Arduino to connect to.
  
  Set time by default is 3 second.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.
*/

#include <WiFiNINA.h>

#define BLINK_DELAY 3000          // time in ms that the LED is on/off

// Details for Wifi network access point
char ssid[] = "WifiNINA-Access-Point";        // your network SSID (name)
char pass[] = "123456789";                    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                             // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

const char serverLed[] = "192.168.4.2";   // LED server IP address 

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {  
  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);  // by default the local IP address of will be 192.168.4.1

  if (status != WL_AP_LISTENING) {
    // creating access point failed
    // don't continue
    while (true);
  }  
}

void loop() {
  client.connect(serverLed, 80);
  while (client.connected()) {                  // if connection to led server successful
    
    client.println("GET /H HTTP/1.1");          // Make a HTTP request to turn ON the LED
    client.println("Host: 192.168.4.2");        // additional details from HTTP request
    client.println("Connection: keep-alive");   // no reply expected
    client.println();                           // send empty new line (end of HTTP request)
    delay(BLINK_DELAY);                         // wait for set delay time 

    client.println("GET /L HTTP/1.1");          // Make a HTTP request to turn OFF the LED
    client.println("Host: 192.168.4.2");        // additional details from HTTP request
    client.println("Connection: keep-alive");   // no reply expected
    client.println();                           // send empty new line (end of HTTP request)
    delay(BLINK_DELAY);                         // wait for set delay time 
  }

  delay(1000);                                  //wait the server to be ready                           
}
