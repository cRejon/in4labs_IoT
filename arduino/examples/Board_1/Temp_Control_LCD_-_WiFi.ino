/*
  Sketch name: Temp Control LCD - WiFi
  UNED Arduino IoT lab. 

  WiFi Temperature and Humidity control - LCD

  This sketch will create a new WiFi access point for other Arduinos to connect to.  

  A client that connects to a web server (Temp Control Sensor - WiFi) to request 
  Temperature and Humidity sensor readings and based on these connects to
  another web server (Temp Control Fan - WiFi) and sends intructions to turns on/off a fan.
  
  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.
  
  Review example 'LCD Test' to see the functionalities of the LCD screen.
*/

#include <WiFiNINA.h>           // include WiFi library
#include <LiquidCrystal.h>      // include LCD library

#define TEMP_LIMIT 50.0    // if this threshold is reached - instruct fan on 
#define HUM_LIMIT 80.0     // if this threshold is reached - instruct fan on 

String fanStatus = "OFF";             // fan status to be printed by LCD screen
String temperatureRead;               // make a String to hold data received from the sensor
String humidityRead;                  // make a String to hold data received from the sensor

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Details for Wifi network access point created by: Temp_Control_Sensor_-_Wifi
char ssid[] = "WifiNINA-Access-Point";  // your network SSID (name)
char pass[] = "123456789";              // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                       // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

const char serverSensor[] = "192.168.4.2";    // sensor server IP address
const char serverFan[] = "192.168.4.3";       // fan server IP address

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {  
  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear();  

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);  // by default the local IP address of will be 192.168.4.1
  
  if (status != WL_AP_LISTENING) {
    // creating access point failed
    // don't continue
    while (true);
  }
}

void loop() {
  int counter = 0;            // attempt counter when waiting from reply from server
  char c;                     // byte of data read from server
  temperatureRead = "";       // clear data for new reading
  humidityRead = "";          // clear data for new reading 

  if (client.connect(serverSensor, 80)) {   // if connection to sensor server successful
    client.println("GET /Temperature HTTP/1.1");  // Make a HTTP request to request temperature
    client.println("Host: 192.168.4.2");          // additional details from HTTP request
    client.println("Connection: keep-alive");
    client.println();                             // send empty new line (end of HTTP request) to server                           
    delay(1);                                     // give client time to receive, process data and reply
    
    while (client.available() == 0 && counter < 100 ){  // wait until feedback from server is received
                                                        // or a max of 100 attempts * 10ms delay have been made
      delay(10);                    // reply from server not yet received. Wait for an extra 10ms
      counter = counter+1;          // attempt counter +1
      if  (counter == 100){         // reply from sensor not received
        temperatureRead = "error";  // display error reading
      }
    }

    if (counter != 100){                // reply from sensor was received
      while (client.available() > 0){   // while there is data from sensor server
        c = client.read();              // read a byte, then
        temperatureRead += c;           // add it into temperatureRead String 
        // take the first 4 characters of the String
        // this is the temperature value
        if (temperatureRead.length() == 4){
          break;
        }
      }  
    }
    counter = 0;                        // reset attempt counter
    
    if (!client.connected()) {          // if the server's disconnected
      client.stop();                    // stop the client
    }  

    delay(1);                           // delay for smoother performance
  }

  if (client.connect(serverSensor, 80)) {       // if connection to sensor server successful
    client.println("GET /Humidity HTTP/1.1");   // Make a HTTP request to request humidity
    client.println("Host: 192.168.4.2");        // additional details from HTTP request
    client.println("Connection: keep-alive");
    client.println();                           // send empty new line (end of HTTP request) to server                           
    delay(1);                                   // give client time to receive, process data and reply

    while (client.available() == 0 && counter < 100 ){  // wait until feedback from server is received
                                                        // or a max of 100 attempts * 10ms delay have been made
      delay(10);                    // reply from server not yet received. Wait for an extra 10ms
      counter = counter+1;          // attempt counter +1
      if  (counter == 100){         // reply from sensor not received
        humidityRead = "error";     // display error reading
      }
    }

    if (counter != 100){                // reply from sensor was received
      while (client.available() > 0){   // while there is data from sensor server
        c = client.read();              // read a byte, then
        humidityRead += c;              // add it into humidityRead String 
        // take the first 4 characters of the String
        // this is the humidity value
        if (humidityRead.length() == 4){
          break;
        }
      }  
    }
    counter = 0;                        // reset attempt counter
    
    if (!client.connected()) {          // if the server's disconnected
      client.stop();                    // stop the client
    }  

    delay(1);                           // delay for smoother performance
  }

  float temperatureFloat = temperatureRead.toFloat(); // convert String data and compare to TEMP_LIMIT
  float humidityFloat = humidityRead.toFloat();       // convert String data and compare to HUM_LIMIT

  if (humidityFloat > HUM_LIMIT || temperatureFloat > TEMP_LIMIT){  // if any threshold is reached                          
    if (client.connect(serverFan, 80)) {      // if connection to fan server successful
      client.println("GET /H HTTP/1.1");      // Make a HTTP request to turn ON the fan
      client.println("Host: 192.168.4.3");    // additional details from HTTP request
      client.println("Connection: close");    // no reply expected
      client.println();                       // send empty new line (end of HTTP request)
     
      fanStatus = "ON";                       // update fan status to be printed by LCD screen
    }
  } else {                                    // if no threshold is reached
    if (client.connect(serverFan, 80)) {      // and if connection to fan server successful
      client.println("GET /L HTTP/1.1");      // Make a HTTP request to turn OFF the fan
      client.println("Host: 192.168.4.3");    // additional details from HTTP request
      client.println("Connection: close");    // no reply expected
      client.println();                       // send empty new line (end of HTTP request)
      
      fanStatus = "OFF";                      // update fan status to be printed by LCD screen
    }
  }
  
  updateLCD();                     
  
  delay(2000);                     // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
}

// function that updates sensor readings and fan status in LCD screen
void updateLCD() {
  // clear LCD screen
  lcd.clear();  
  // sensor readings
  lcd.setCursor(1, 0);
  lcd.print("T:");
  lcd.setCursor(3, 0);
  lcd.print(temperatureRead);
  lcd.setCursor(7, 0);
  lcd.print(" C");
  
  lcd.setCursor(1, 1);
  lcd.print("H:");
  lcd.setCursor(3, 1);
  lcd.print(humidityRead);
  lcd.setCursor(7, 1);
  lcd.print(" %");
 
  // separation line
  lcd.setCursor(10, 0);
  lcd.print("|"); 
  lcd.setCursor(10, 1);
  lcd.print("|"); 
 
  // fan status
  lcd.setCursor(12, 0);
  lcd.print("Fan");
  lcd.setCursor(12, 1);
  lcd.print(fanStatus);
}
