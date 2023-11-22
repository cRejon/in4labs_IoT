#include <ArduinoBLE.h>
#include <DHT.h>

#define DHT22_PIN 2                 // define the type data pin
#define DHTType DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DHT22_PIN, DHTType);    // instantiate the dht class with our data pin and DHT type.

unsigned long previousMillis = 0;     // will store last time DHT was sampled

// ID DEFINITIONS FOR BLE COMMUNICATION:
BLEService sensorService("181A");    // BLE Sensor Service

BLEIntCharacteristic tempCharacteristic("2A6E", BLERead);  // BLE Temperature Sensor Characteristic
BLEIntCharacteristic humCharacteristic("2A6F", BLERead);   // BLE Humidity Sensor Characteristic
     

void setup() {

  dht.begin();                // call the begin class in the dht object

  BLE.begin();                  // begin initialization
  BLE.setLocalName("Arduino DHT22 Sensor");    // set the local name peripheral advertises
  BLE.setAdvertisedService(sensorService);     // set the UUID for the service this peripheral advertises

  sensorService.addCharacteristic(tempCharacteristic); // add the temperature sensor characteristic to sensorService
  sensorService.addCharacteristic(humCharacteristic);    // add the humidity sensor characteristic to sensorService
  
  BLE.addService(sensorService);    // add the service

  tempCharacteristic.writeValue(0);   // set initial values for each characteristic
  humCharacteristic.writeValue(0);

  BLE.advertise();      // start advertising
}

void loop() {
  
  BLEDevice central = BLE.central();      // wait for BLE central devices to connect

  if (central) {                          // run if a central is connected to sensor peripheral
    while (central.connected()) {         // while the central is still connected to fan peripheral
      long currentMillis = millis();      // current time to be compared with previousMillis
      
      if (currentMillis - previousMillis >= 2000) {    // if >2 seconds from last reading. DHT22 sampling rate is 0.5Hz
        float temp = dht.readTemperature();            
        int tempInt = (int) (temp*10);
        tempCharacteristic.writeValue(tempInt);   // update the temperature characteristic
        
        float hum = dht.readHumidity(); 
        int humInt = (int) (hum*10);
        humCharacteristic.writeValue(humInt);     // update the humidity characteristic

        previousMillis = currentMillis;                // reset previousMillis
      }
    }
  }
}
