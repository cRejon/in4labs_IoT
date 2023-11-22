/*
  Sketch name: Temp Control LCD - BLE
  UNED Arduino IoT lab. 

  BLE Temperature and Humidity control - LCD

  A BLE central device that connects with BLE pheripherals and reads characteristics 
  including Temperature and Humidity sensor readings (Temp Control Sensor - BLE)
  and modifies characteristics on (Temp Control Fan - BLE) to turn on/off a fan.
 
  Review example 'LCD Test' to see the connections and functionalities of the LCD screen.
*/

#include <ArduinoBLE.h>         // include BLE library
#include <LiquidCrystal.h>      // include LCD library

#define TEMP_LIMIT 50.0    // if this threshold is reached - instruct fan on 
#define HUM_LIMIT 80.0     // if this threshold is reached - instruct fan on 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String fanStatus = "OFF";       // fan status to be printed by LCD screen
String temperatureRead;         // make a String to hold data received from the sensor
String humidityRead;            // make a String to hold data received from the sensor
float temperatureFloat;         // make a float to convert String data and compare to TEMP_LIMIT
float humidityFloat;            // make a float to convert String data and compare to HUM_LIMIT

BLEDevice peripheral;                       // variable referencing a BLE peripheral device
BLECharacteristic tempCharacteristic; // variable referencing a characteristic from a BLE peripheral device
BLECharacteristic humCharacteristic; // variable referencing a characteristic from a BLE peripheral device


void setup() {
  BLE.begin();                  // initialization of BLE communication

  lcd.begin(16, 2);             // set up the LCD's number of columns and rows
  lcd.clear(); 
}

void loop() {
  temperatureRead = "";         // clear data for new reading
  humidityRead = "";            // clear data for new reading
  BLE.scanForUuid("181F");      // scan and target BLE Sensor Service
  peripheral = BLE.available(); // BLE peripheral device discovered // false if not discovered

  int counter = 0;              // setup counter for amount of connection attempts
  
  while(!peripheral && counter < 100){  // while peripheral not discovered and less than 100 attempts
    BLE.scanForUuid("181F");            // target BLE Sensor Service
    delay(10);                          // delay time to find device         
    peripheral = BLE.available();       // BLE peripheral device discovered // false if not discovered        
    counter =counter+1;                 // increase attempt counter
  }  

  BLE.stopScan();                        // stop scanning

  if (peripheral.connect()) {  // connecting to peripheral
    delay(10);                          // small delay to smooth communication
  
    if (peripheral.discoverService("181F")) {  // connecting to sensor service
      // retrieve the temperature characteristic
      char tempChar [5] = {0};      
      tempCharacteristic = peripheral.characteristic("2A21"); 
      tempCharacteristic.readValue(tempChar, 4); // read temperature into char array
      for (int i = 0; i < 4; i++){                          // transfer char array into String
        temperatureRead += tempChar[i]; 
      }
      delay(100); 
      // retrieve the humidity characteristic
      char humChar [5] = {0};
      humCharacteristic = peripheral.characteristic("2A20");
      humCharacteristic.readValue(humChar, 4); // read humidity into char array
      for (int i = 0; i < 4; i++){                          // transfer char array into String
        humidityRead += humChar[i]; 
      }
    }
    
    peripheral.disconnect();            // disconnect from peripheral       
         
    updateLCD();                        // dispaly sensor readings in LCD screen  
  }
  
  temperatureFloat = temperatureRead.toFloat(); // convert String data and compare to TEMP_LIMIT
  humidityFloat = humidityRead.toFloat();       // convert String data and compare to HUM_LIMIT
  
  if ( humidityFloat > HUM_LIMIT || temperatureFloat > TEMP_LIMIT){ // if any threshold is reached
    instructFan(0x01);          // call function to write 0x01 into fan BLE device - fan on
    fanStatus = "ON";           // update fan status to be printed by LCD screen
    
  } else {
    instructFan(0x00);          // call function to write 0x00 into fan BLE device - fan off
    fanStatus = "OFF";          // update fan status to be printed by LCD screen
  }

  updateLCD();                     // dispaly new fan status in LCD screen
  
  delay(2000);                     // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
}

void instructFan (byte value){  // function to write input "value" into BLE fan device
                                // switchCharacteristic. 0x00 will turn fan off
                                // any other value will turn it on. 
                                // see example Temp_Control_Fan_-_BLE

  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");  // scan and target BLE Fan Service
  
  peripheral = BLE.available(); // BLE peripheral device discovered // false if not discovered

  int counter = 0;              // setup counter for amount of connection attempts
  
  while(!peripheral && counter < 100){  // while peripheral not discovered and less than 100 attempts
    BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");            
                                        // target BLE Fan Service
    delay(10);                          // delay time to find device         
    peripheral = BLE.available();       // BLE peripheral device discovered // false if not discovered        
    counter =counter+1;                 // increase attempt counter
  }  
  BLE.stopScan();                       // stop scanning

  if (!peripheral.connect()) {  // connecting to peripheral
                                // skip if connection unsuccessful
  } else {                      // connection successful

  delay(10);                          // small delay to smooth communication

    if (!peripheral.discoverService("19B10000-E8F2-537E-4F6C-D104768A1214")) {  
                                      // connecting to FAN service
                                      // skip if connection unsuccessful
    } else {                          // connection successful

      // retrieve the FAN switchCharacteristic
      BLECharacteristic peripheralCharacteristic = peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1214");
      peripheralCharacteristic.writeValue((byte)value);
      // write input "value" into FAN switchCharacteristic  
    }
      
    peripheral.disconnect();            // disconnect from peripheral       
  }
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