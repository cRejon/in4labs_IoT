/*
    Sketch name: Temp Control LCD - WiFi
    UNED Arduino IoT lab. 

    WiFi Temperature and Humidity control - LCD

    This sketch will create a new WiFi access point for other Arduinos to connect to.  

    A client that connects to a web server (Temp Control Sensor - WiFi) to request 
    Temperature and Humidity sensor readings and based on these connects to
    another web server (Temp Control Fan - WiFi) and sends intructions to turns on/off a fan.
    
    Review example 'LCD Test' to see the functionalities of the LCD screen.
*/

#include <WiFi.h>               // include WiFi library
#include <LiquidCrystal.h>      // include LCD library

#define TEMP_LIMIT 50.0    // if this threshold is reached - instruct fan on 
#define FAN_ON 0x01            // value to write to fan device to turn it on
#define FAN_OFF 0x00           // value to write to fan device to turn it off

byte fanStatus = FAN_OFF;           // fan status
byte previousFanStatus = FAN_OFF;   // previous fan status
String temperatureReading;               // char array to hold temperature reading
String humidityReading;                // char array to hold humidity reading

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Details for Wifi network access point created by: Temp_Control_Sensor_-_Wifi
char ssid[] = "WiFi-Access-Point";  // your network SSID (name)
char password[] = "123456789";              // your network password 

const char serverSensor[] = "192.168.4.2";    // sensor server IP address
const char serverFan[] = "192.168.4.3";       // fan server IP address

// Initialize the WiFi client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {  
    lcd.begin(16, 2);           // set up the LCD's number of columns and rows
    lcd.clear();  
    lcd.setCursor(1, 0);
    lcd.print("Connecting..."); // advertisement message

    // Create open network, by default the local IP address of will be 192.168.4.1
    if (!WiFi.softAP(ssid, password)) { 
        // Creating access point failed, don't continue
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("AP Setup Failed");
        while (true); // Halt the program
    }  
}

void loop() {

    temperatureReading = getSensorData(serverSensor, "temperature");  // request temperature reading from sensor server
    humidityReading = getSensorData(serverSensor, "humidity");      // request humidity reading from sensor server
    
    // convert temperatureReading to float and compare to TEMP_LIMIT
    float temperature = temperatureReading.toFloat();      // convert temperature reading to float
    if (temperature > TEMP_LIMIT){               // if the threshold is reached 
        fanStatus = FAN_ON;                                 // activate fan
    } else {
        fanStatus = FAN_OFF;          
    }  

    if (fanStatus != previousFanStatus) {
        sendFanControlRequest(serverFan, fanStatus == FAN_ON ? "H" : "L");
        previousFanStatus = fanStatus;
    }
    
    updateLCD();                     
    
    delay(2000);                     // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
}

String getSensorData(const char* server, const char* request) {
    String dataRead;
    if (client.connect(server, 80)) {
        sendHttpRequest(server, request);
        dataRead = readHttpResponse();
    }
    return dataRead.substring(0, 4);  // Taking only the first 4 characters as the value
}

// function that sends a request to the server
void sendHttpRequest(const char* server, const char* request) {
    client.print("GET /");
    client.print(request);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: keep-alive");
    client.println();     // send empty new line (end of HTTP request)
    delay(1);            // give client time to receive, process data and reply
}

// fucntion that reads the HTTP response from the server
String readHttpResponse() {
    String response;
    int counter = 0;

    while (client.available() == 0 && counter < 100) {
        delay(10);
        counter++;
    }

    while (client.available()) {
        char c = client.read();
        response += c;
    }

    client.stop();
    return response;
}

// function that sends a request to the fan server
void sendFanControlRequest(const char* server, const char* command) {
    if (client.connect(server, 80)) {
        client.print("GET /");
        client.print(command);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(server);
        client.println("Connection: close");   // no reply expected
        client.println();                      // send empty new line (end of HTTP request)
        delay(1);                              // give client time to receive the data
        client.stop();                         // close the connection
    }
}

// function that updates sensor readings and fan status in LCD screen
void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("T:");
    lcd.print(temperatureReading);
    lcd.print(" C");

    lcd.setCursor(1, 1);
    lcd.print("H:");
    lcd.print(humidityReading);
    lcd.print(" %");

    lcd.setCursor(10, 0);
    lcd.print("|"); 
    lcd.setCursor(10, 1);
    lcd.print("|"); 
 
    lcd.setCursor(12, 0);
    lcd.print("Fan");
    lcd.setCursor(12, 1);
    lcd.print(fanStatus == FAN_ON ? "ON" : "OFF");
}
