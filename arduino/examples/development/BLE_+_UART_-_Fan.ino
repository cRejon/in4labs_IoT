#include <ArduinoBLE.h>         // include BLE library

String fanStatus = "OFF";       // fan status to be printed by LCD screen
String tempRead;         // make a String to hold data received from the sensor
String humidityRead;            // make a String to hold data received from the sensor

BLEDevice peripheral;                       // variable referencing a BLE peripheral device
BLECharacteristic tempCharacteristic; // variable referencing a characteristic from a BLE peripheral device
BLECharacteristic humCharacteristic; // variable referencing a characteristic from a BLE peripheral device


void setup() {
  BLE.begin();                  // initialization of BLE communication

  Serial1.begin(115200);           // initialize UART at 115200 bauds
  while (!Serial1) {
    ;                              // Serial used for communication with other boards
  }
}

void loop() {
  tempRead = "";         // clear data for new reading
  humidityRead = "";            // clear data for new reading
  
  do {
    BLE.scanForName("DHT22SensorTest");            // target BLE Sensor Service
    peripheral = BLE.available();
    delay(10);
  } while (!peripheral);

  BLE.stopScan();                        // stop scanning

  if (peripheral.connect()) {  // connecting to peripheral
    delay(10);                          // small delay to smooth communication
  
    if (peripheral.discoverService("181F")) {  // connecting to sensor service
      // retrieve the temperature characteristic
      char tempChar [5] = {0};      
      tempCharacteristic = peripheral.characteristic("2A21"); 
      tempCharacteristic.readValue(tempChar, 4); // read temperature into char array
      for (int i = 0; i < 4; i++){                          // transfer char array into String
        tempRead += tempChar[i]; 
      }
      

      Serial1.write(tempChar, 4);
      Serial1.write("20.5");

      
      Serial1.flush();
      delay(100); 
      // retrieve the humidity characteristic
      char humChar [5] = {0};
      humCharacteristic = peripheral.characteristic("2A20");
      humCharacteristic.readValue(humChar, 4); // read humidity into char array
      for (int i = 0; i < 4; i++){                          // transfer char array into String
        humidityRead += humChar[i]; 
      }
      //Serial1.Write(humidityRead);
    }
    
    peripheral.disconnect();            // disconnect from peripheral       
         
  }
  

  delay(2000);                     // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
}

