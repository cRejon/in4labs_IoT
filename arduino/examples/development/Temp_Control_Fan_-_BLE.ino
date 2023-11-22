/*
  Sketch name: Temp Control Fan - BLE
  UNED Arduino IoT lab.

  BLE Temperature and Humidity control - Fan

  This example creates a BLE (Bluthooth Low Energy) peripheral with a service that 
  contains a characteristic that can be modified by a central peripheral control a FAN. 
  Central device programmed with 'Temp Control TFT - BLE' example.
  
  Circuit:
    - Fan (+) conected to 12 V (+)
    - Fan (-) conected to Relay NC (Normally Closed) 
    - Relay COM (common) to ground
    - Pin 2 connected to Relay Signal pin
  
  Review example 'Fan Test' for a detailed description of how this fan works.
*/

#include <ArduinoBLE.h>

#define FAN_PIN 2                 // define the pin connected to fan

// ID DEFINITIONS FOR BLE COMMUNICATION:
// ID for BLE Fan Service
BLEService fanService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
// BLE FAN Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  if (!BLE.begin()) {                           // initialization of BLE communication
    while (1);
  }

  BLE.setLocalName("Arduino Fan");                              // set advertised local name of this peripheral
  BLE.setAdvertisedService(fanService);                 // set service UUID for this peripheral
  fanService.addCharacteristic(switchCharacteristic);   // add the switchCharacteristic to the fanService
  BLE.addService(fanService);                           // add fanService to this peripheral
  switchCharacteristic.writeValue(0x00);   // set the initial value for the characeristic switchCharacteristic
  BLE.advertise();                      // start advertising this BLE peripheral to remote central devices

  pinMode(FAN_PIN, OUTPUT);               // set the FAN pin as an output
  digitalWrite(FAN_PIN, HIGH);          // fan off
}

void loop() {                   // wait for BLE central devices to connect
  
  BLEDevice central = BLE.central();

  if (central) {                // run if a central is connected to fan peripheral
    while (central.connected()) {       // while the central is still connected to fan peripheral
      if (switchCharacteristic.written()) {   // if the remote device wrote to the characteristic,
                                              // use the new characteristic value to control the FAN
        if (switchCharacteristic.value()) {   // any value other than 0x00 will turn the FAN on
          digitalWrite(FAN_PIN, LOW);         
        } else {                              // a 0x00 value will turn the FAN off
          digitalWrite(FAN_PIN, HIGH);        
        }
      }
    }
  }
}
