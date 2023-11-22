#include <ArduinoBLE.h>         // include BLE library

String fanStatus = "OFF";       // fan status to be printed by LCD screen
char tempRead[5]; 
char humRead[5];
float tempFloat = 0.0;        // variable to hold temperature data
float humFloat = 0.0;        // variable to hold temperature data

BLEDevice peripheral;                       // variable referencing a BLE peripheral device
BLECharacteristic tempCharacteristic; // variable referencing a characteristic from a BLE peripheral device
BLECharacteristic humCharacteristic; // variable referencing a characteristic from a BLE peripheral device


void setup() {
  BLE.begin();                  // initialization of BLE communication

  Serial.begin(9600);           // initialize UART at 115200 bauds

}

void loop() {
  
  do {
    BLE.scanForName("Arduino DHT22 Sensor");            // target BLE Sensor Service
    peripheral = BLE.available();
    delay(10);
  } while (!peripheral);

  BLE.stopScan();                        // stop scanning

  if (peripheral.connect()) {  // connecting to peripheral
    delay(10);                          // small delay to smooth communication
  
    if (peripheral.discoverService("181A")) {  // connecting to sensor service
      // retrieve the temperature characteristic
      int temp = 0;      
      tempCharacteristic = peripheral.characteristic("2A6E"); 
      tempCharacteristic.readValue(temp); 
      tempFloat = temp / 10.0;    // convert to float and divide by 10
      dtostrf(tempFloat, 4, 1, tempRead);  // convert to string with 1 decimal place
      Serial.print("Temperature: ");
      Serial.println(tempRead);
      delay(100); 

      // retrieve the humidity characteristic
      int hum = 0;
      humCharacteristic = peripheral.characteristic("2A6F");
      humCharacteristic.readValue(hum);
      humFloat = hum / 10.0;
      dtostrf(humFloat, 4, 1, humRead);  // convert to string with 1 decimal place
      Serial.print("Humidity: ");
      Serial.println(humRead);
    }
    
    peripheral.disconnect();            // disconnect from peripheral       
         
  }
  

  delay(2000);                     // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
}

