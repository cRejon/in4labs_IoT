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
#define ON 0x01            // value to write to BLE fan device to turn it on
#define OFF 0x00           // value to write to BLE fan device to turn it off

byte fanStatus = OFF;           // fan status
byte previousFanStatus = OFF;   // previous fan status
String tempRead;               // char array to hold temperature reading
String humRead;                // char array to hold humidity reading

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
  // advertisement message
  lcd.setCursor(1, 0);
  lcd.print("Connecting...");

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);  // by default the local IP address of will be 192.168.4.1
  
  while (status != WL_AP_LISTENING) {
    delay(500);
    status = WiFi.beginAP(ssid, pass);
  }
}

void loop() {

  tempRead = getSensorData(serverSensor, "temperature");  // request temperature reading from sensor server
  humRead = getSensorData(serverSensor, "humidity");      // request humidity reading from sensor server
  
  // convert tempRead to float and compare to TEMP_LIMIT
  float temperatureFloat = tempRead.toFloat();      // convert temperature reading to float
  if (temperatureFloat > TEMP_LIMIT){               // if the threshold is reached 
    fanStatus = ON;                                 // activate fan
  } else {
    fanStatus = OFF;          
  }  

  if (fanStatus != previousFanStatus){  // if any threshold is reached                          
    if (client.connect(serverFan, 80)) {      // if connection to fan server successful
      if (fanStatus == ON)
      {
        client.println("GET /H HTTP/1.1");      // Make a HTTP request to turn ON the fan
      } else {
        client.println("GET /L HTTP/1.1");      // Make a HTTP request to turn OFF the fan
      }
      client.print("Host: ");    // additional details from HTTP request
      client.println(serverFan);    // additional details from HTTP request
      client.println("Connection: close");    // no reply expected
      client.println();                       // send empty new line (end of HTTP request)
    }
  }
  
  updateLCD();                     
  
  delay(2000);                     // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
}

String getSensorData(const char* server, const char* request) {
    int counter = 0;            // attempt counter when waiting from reply from server
    char c;                     // byte of data read from server
    String dataRead;            

    if (client.connect(server, 80)) {   // if connection to sensor server successful
        client.print("GET /");          // Make a HTTP request
        client.print(request);          
        client.println(" HTTP/1.1");          
        client.print("Host: ");          // additional details from HTTP request
        client.println(server);          
        client.println("Connection: keep-alive");
        client.println();                             // send empty new line (end of HTTP request) to server
        delay(1);                                     // give client time to receive, process data and reply

        while (client.available() == 0 && counter < 100 ){  // wait until feedback from server is received
                                                                                                                // or a max of 100 attempts * 10ms delay have been made
            delay(10);                    // reply from server not yet received. Wait for an extra 10ms
            counter = counter+1;          // attempt counter +1
        }

        if (counter != 100){                // reply from sensor was received
        while (client.available() > 0){   // while there is data from sensor server
            c = client.read();              // read a byte, then
            dataRead += c;           // add it into temperatureRead String 
            // take the first 4 characters of the String
            // this is the temperature value
            if (dataRead.length() == 4){
            break;
            }
        }  
        }
    }
    return dataRead;
}

// function that updates sensor readings and fan status in LCD screen
void updateLCD() {
  // clear LCD screen
  lcd.clear();  
  // sensor readings
  lcd.setCursor(1, 0);
  lcd.print("T:");
  lcd.setCursor(3, 0);
  lcd.print(tempRead);
  lcd.setCursor(7, 0);
  lcd.print(" C");
  
  lcd.setCursor(1, 1);
  lcd.print("H:");
  lcd.setCursor(3, 1);
  lcd.print(humRead);
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
  if (fanStatus == ON) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }
}
