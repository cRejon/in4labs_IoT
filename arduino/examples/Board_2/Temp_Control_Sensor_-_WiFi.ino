/*
  Sketch name: Temp Control Sensor - WiFi
  UNED Arduino IoT lab. 

  WiFi Temperature and Humidity control - Sensor

  A simple web server that shows the value of the temperature and humidity readings 
  from sensor DHT22.

  This sketch will connect to an access point created by sketch: Temp Control LCD - WiFi
  
  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  Circuit:
    - DATA_PIN 2 connected to sensor data line.

  Review example 'DHT22 Sensor Test' for a detailed description of how this sensor works.
*/

#include <WiFiNINA.h>
#include <DHT.h>    

#define DATA_PIN 2                    // define the type data pin
#define DHT_TYPE DHT22                // define the DHT sensor (DHT11, DHT21, or DHT22)


DHT dht = DHT(DATA_PIN, DHT_TYPE);    // instantiate the dht class with our data pin and DHT type.

// Details for Wifi network access point:
char ssid[] = "WifiNINA-Access-Point";        // your network SSID (name)
char pass[] = "123456789";                    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                             // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);        // server on port 80

void setup() {

  dht.begin();                // call the begin class in the dht object

  WiFi.config(IPAddress(192, 168, 4, 2)); // set IP address
  while (status != WL_CONNECTED) {        // attempt to connect to Wifi network
    status = WiFi.begin(ssid, pass);      // connect to WPA/WPA2 network created by: Temp_Control_Sensor_-_Wifi
    delay(1000);                          // wait 1 seconds before next connection attempt
  } 
  
  server.begin();                     // start the web server on port 80
}


void loop() {
  String sensorRead;                        // make a String to hold data read from the sensor
  
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
          if (sensorRead != 0){         // output the value of sensor  
            client.print(sensorRead);   // send value of sensor to client      
            client.println();           // send empty new line (end of reply) to client 
            sensorRead = "";            // clear sensorRead before next request
          }
          break;
        }
        if (c == '\n') {                // if you got a newline, then clear currentLine:  
          currentLine = "";
        } else if (c != '\r') {         // if you got anything else but a carriage return character,
          currentLine += c;             // add it to the end of the currentLine
        } 
        
        if (currentLine.endsWith("GET /Temperature")){      // client request for Temperature
          float temp = dht.readTemperature();               // read sensor Temperature
          sensorRead = String(temp, 2);                     // convert float to String
        } else if (currentLine.endsWith("GET /Humidity")){  // client request for Humidity
          float hum = dht.readHumidity();                   // read sensor Humidity
          sensorRead = String(hum, 2);                      // convert float to String
        }
      }
    }
    delay(1);    // give client time to receive the data

    client.stop();                        // close the connection:
  }

  // check if WiFi is still connected, if not, reconnect
  if (WiFi.status() != WL_CONNECTED) {
    setup();
  }
}
