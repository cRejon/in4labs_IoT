/*
  Sketch name: Slave Blink - BLE
  UNED Arduino IoT lab.
 
  BLE (Bluetooth Low Energy) peripheral acting as a Slave that, under commands 
  from Central device programmed with 'Master Blink - BLE' example, turns an LED on 
  for a set delay time, then off, repeatedly.

  Circuit:
    - RGB LED connected to pins 5 (red), 6 (green) and 9 (blue)
*/

#include <ArduinoBLE.h>

#define RGB_PIN 5    

// ID definitions for BLE communication:
// ID for BLE Led Service
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);


void setup() {
  if (!BLE.begin()) {     // initialization of BLE communication
    while (1);
  }

  BLE.setLocalName("LED");                              // set advertised local name of this peripheral
  BLE.setAdvertisedService(ledService);                 // set service UUID for this peripheral
  ledService.addCharacteristic(switchCharacteristic);   // add the switchCharacteristic to the Service
  BLE.addService(ledService);                           // add ledService to this peripheral
  switchCharacteristic.writeValue(0);   // set the initial value for the characeristic switchCharacteristic
  BLE.advertise();                      // start advertising this BLE peripheral to remote central devices
  
  pinMode(RGB_PIN, OUTPUT);             // initialize digital pin RGB_PIN as an output
  digitalWrite(RGB_PIN, LOW);           // start with the LED off 
}

void loop() {                           // wait for BLE central devices to connect
  
  BLEDevice central = BLE.central();

  if (central) {                        // run if a central is connected to led peripheral

    while (central.connected()) {             // while the central is still connected to led peripheral
      if (switchCharacteristic.written()) {   // if the remote device wrote to the characteristic,
                                              // use the new characteristic value to control the LED
        if (switchCharacteristic.value()) {   // any value other than 0
          digitalWrite(RGB_PIN, HIGH);        // turn the LED on 
        } else {                              // a 0 value
          digitalWrite(RGB_PIN, LOW);         // turn the LED off 
        }
      }
    }
  }
}
