#include <ArduinoBLE.h>         // include BLE library
#include <LiquidCrystal.h>      // include LCD library

#define TEMP_LIMIT 50.0    // if this threshold is reached - instruct fan on 
#define ON 0x01            // value to write to BLE fan device to turn it on
#define OFF 0x00           // value to write to BLE fan device to turn it off

byte fanStatus = OFF;
byte previousFanStatus = OFF; 
char tempRead[5]; 
char humRead[5];
float tempFloat = 0.0;        // variable to hold temperature data
float humFloat = 0.0;        // variable to hold temperature data

BLEDevice peripheral;                       // variable referencing a BLE peripheral device
BLECharacteristic tempCharacteristic; // variable referencing a characteristic from a BLE peripheral device
BLECharacteristic humCharacteristic; // variable referencing a characteristic from a BLE peripheral device


void setup() {
  BLE.begin();                  // initialization of BLE communication

  Serial.begin(9600);

}

void loop() {
  
  do {
    BLE.scanForName("Arduino DHT22 Sensor");   // target BLE Sensor Service
    peripheral = BLE.available();
    delay(10);
  } while (!peripheral);

  Serial.print("Found ");
  Serial.print(peripheral.address());
  Serial.print(" '");
  Serial.print(peripheral.localName());
  Serial.print("' ");
  Serial.print(peripheral.advertisedServiceUuid());
  Serial.println();

  BLE.stopScan();                        // stop scanning

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  tempCharacteristic = peripheral.characteristic("2A6E"); 
  humCharacteristic = peripheral.characteristic("2A6F");

  while (peripheral.connected()) {    

    // retrieve the temperature characteristic
    int temp = 0;
    tempCharacteristic.readValue(temp);
    tempFloat = temp / 10.0;    // convert to float and divide by 10
    dtostrf(tempFloat, 4, 1, tempRead);  // convert to string with 1 decimal place
    Serial.print("Temperature: ");
    Serial.print(tempRead);
    //delay(100); 
    // retrieve the humidity characteristic
    int hum = 0;
    humCharacteristic.readValue(hum);
    humFloat = hum / 10.0;
    dtostrf(humFloat, 4, 1, humRead);  // convert to string with 1 decimal place
    Serial.print(" Humidity: ");
    Serial.println(humRead);
    
    
    if ( tempFloat > TEMP_LIMIT ){ // if the temperature threshold is reached
        fanStatus = ON;           // update fan status to be printed by LCD screen
    } else {
        fanStatus = OFF;          // update fan status to be printed by LCD screen
    }
    
    Serial.print("Fan status: ");
    Serial.println(fanStatus, HEX);

    if (fanStatus != previousFanStatus){ // if fan status has changed
      peripheral.disconnect();            // disconnect from peripheral  
      previousFanStatus = fanStatus;      // update previous fan status
    }      
  }

  delay(2000); // delay between reads
}

