/*
  Sketch name: Temp Control LCD - RS485
  UNED Arduino IoT lab.

  RS485 Temperature and Humidity control - LCD

  This example creates a master that obtains sensor data from a 
  slave (Temp Control Sensor - RS485), presents the information in a 
  LCD screen and instructs slave (Temp Control Fan - RS485) to 
  turn on/off a fan.
  
  Communication is done via RS-485. Arduinos are conected via SoftwareSerial
  from pins 7 (RX_PIN) and 8 (TX_PIN) to modules MAX485 which are then connected
  to each other via an RS-485 bus.
  
  Circuit (RS485):
    - Sofware RX (pin 7) connected to MAX485 RO (Receiver Out)
    - Sofware TX (pin 8) connected to MAX485 DI (Driver In)
    - Pin 3 connected to MAX485 DE and RE. When set HIGH module is sending data.
      When is set LOW, module is receiving data.
      
  Review example 'LCD Test' to see the functionalities of the LCD screen.
 */

#include <SoftwareSerial.h>     // include SoftwareSerial library
#include <LiquidCrystal.h>      // include LCD library

#define TEMP_LIMIT 50.00    // if this threshold is reached - instruct fan on 
#define HUM_LIMIT 80.00     // if this threshold is reached - instruct fan on 
//SoftwareSerial pins
#define RX_PIN 7                       // connected to RO (Receiver Out)
#define TX_PIN 8                       // connected to DI (Driver In)
#define TRANSMIT_ENABLER 3             // high to send data. low to receive

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

SoftwareSerial RS485 (RX_PIN, TX_PIN); // assign name, RX and TX pins to software serial

String fanStatus = "OFF";             // fan status to be printed by LCD screen
String temperatureRead;               // make a String to hold data received from the sensor
String humidityRead;                  // make a String to hold data received from the sensor

void setup() {
  RS485.begin(9600);  // initialize serial and wait for port to open:
  while (!RS485) {
    ; // Serial used for communication with other boards
  }
  
  pinMode(TRANSMIT_ENABLER, OUTPUT);
  digitalWrite(TRANSMIT_ENABLER, HIGH); //setup as master

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear();
}

void loop() {
  float temperatureFloat;     // make a float to convert String data and compare to TEMP_LIMIT
  float humidityFloat;        // make a float to convert String data and compare to HUM_LIMIT
  temperatureRead = "";       // clear data for new reading
  humidityRead = "";          // clear data for new reading 
  
  RS485.write("temperature");         // send request to obtain temperature sensor reading
  RS485.flush();                      // wait for data to be sent
  digitalWrite(TRANSMIT_ENABLER,LOW); // disable transmittion. MAX485 receiving data
  delay(100);                         // leave time for transmision to be received and replied generated
  if (RS485.available()){             // if feedback from sensor is received
    delay(1);                         // allow time for message to be received in full
    while (RS485.available()){        // accumulate char bytes of data from slave in String
      char c = RS485.read();
      temperatureRead += c;
    }
  }     
  
  digitalWrite(TRANSMIT_ENABLER, HIGH); // enable transmittion. MAX485 sending data
  RS485.write("humidity");              // send request to obtain humidity sensor reading
  RS485.flush();
  digitalWrite(TRANSMIT_ENABLER, LOW);  // disable transmittion. MAX485 receiving data
  delay(100);                           // leave time for transmision to be received and replied generated
  if (RS485.available()){               // if feedback from sensor is received
    delay(1);                           // allow time for message to be received in full
    while (RS485.available()){          // accumulate char bytes of data from slave in String
      char c = RS485.read();
      humidityRead += c;
    }
  }    
  
  digitalWrite(TRANSMIT_ENABLER,HIGH); // enable transmittion. MAX485 sending data
  
  temperatureFloat = temperatureRead.toFloat(); // convert String data and compare to TEMP_LIMIT
  humidityFloat = humidityRead.toFloat();       // convert String data and compare to HUM_LIMIT

  if (humidityFloat > HUM_LIMIT || temperatureFloat > TEMP_LIMIT){ // if any threshold is reached                   
    RS485.write("turnFanOn");          // send instruction to slaves to turn fan on
    RS485.flush();
    fanStatus = "ON";                 // update fan status to be printed by LCD screen
    
  } else {                        // if no threshold is reached
    RS485.write("turnFanOff");          // send instruction to slaves to turn fan off
    RS485.flush();
    fanStatus = "OFF";                  // update fan status to be printed by LCD screen
  }
  
  updateLCD();                     // dispaly new fan status in LCD screen
  
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
  lcd.setCursor(8, 0);
  lcd.print("C");
  
  lcd.setCursor(1, 1);
  lcd.print("H:");
  lcd.setCursor(3, 1);
  lcd.print(humidityRead);
  lcd.setCursor(8, 1);
  lcd.print("%");
 
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
