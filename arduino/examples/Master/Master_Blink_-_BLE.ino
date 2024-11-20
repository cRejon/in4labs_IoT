/*
  Sketch name: Master Blink - BLE
  UNED Arduino IoT lab.

  A BLE (Bluetooth Low Energy) central device acting as Master that turns an LED 
  on for a set delay time, then off repeatedly, from a peripheral with the 
  'Slave Blink - BLE' example loaded.

  The default blink time is 2 seconds.
*/

#include <ArduinoBLE.h>

#define BLINK_DELAY 2000          // time in ms that the LED is on/off

void setup() {
  Serial.begin(9600);
  // Uncomment this while statement to get debug info from beginning. The master will not 
  // attempt to connect to the slave until the Serial Monitor button is pressed.
  //while (!Serial);

  // initialize the BLE hardware
  BLE.begin();

  // start scanning for peripherals
  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral && peripheral.localName() == "LED") {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

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

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1214");

  if (!ledCharacteristic) {
    Serial.println("Peripheral does not have LED characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {      // while the peripheral is connected
    ledCharacteristic.writeValue(0x01); // Turn LED on
    Serial.println("LED ON");
    delay(BLINK_DELAY);                 // wait for set delay time 
    ledCharacteristic.writeValue(0x00); // Turn LED off
    Serial.println("LED OFF");
    delay(BLINK_DELAY);                 // wait for set delay time
  }

  Serial.println("Peripheral disconnected");
}

