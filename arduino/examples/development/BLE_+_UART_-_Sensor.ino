/*
  Sketch name: Temp Control Sensor - BLE
  UNED Arduino IoT lab. 

  BLE Temperature and Humidity control - Sensor
  
  This example creates a BLE (Bluthooth Low Energy) peripheral that contains 
  characteristics readable by a central device which contain the value of 
  the temperature and humidity readings from the sensor DHT22.
  Central device programmed with 'Temp Control TFT - BLE' example.
   
  Circuit:
    - DHT22_PIN (pin 2) connected to sensor data line

  Review example 'DHT22 Sensor Test' for a detailed description of how this sensor works.
*/

#include <ArduinoBLE.h>
#include <DHT.h>

#define DHT22_PIN 2                 // define the type data pin
#define DHTType DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DHT22_PIN, DHTType);    // instantiate the dht class with our data pin and DHT type.

// ID DEFINITIONS FOR BLE COMMUNICATION:
// BLE Sensor Service
BLEService sensorService("181F");
// BLE Temperature Sensor Characteristic
BLECharacteristic temperatureCharac("2A21",  // standard 16-bit characteristic UUID
    BLERead | BLENotify, 4); // remote clients will be able to get notifications if this characteristic changes
// BLE Humidity Sensor Characteristic
BLECharacteristic humidityCharac("2A20",    // standard 16-bit characteristic UUID
    BLERead | BLENotify, 4); // remote clients will be able to get notifications if this characteristic changes

long previousMillis = 0;                // last time the sensor was checked, in ms

void setup() {
  if (!BLE.begin()) {                           // initialization of BLE communication
    while (1);
  }

  BLE.setLocalName("DHT22SensorTest");               // set advertised local name of this peripheral
  BLE.setAdvertisedService(sensorService);          // set service UUID for this peripheral
  sensorService.addCharacteristic(temperatureCharac); // add the temperature sensor characteristic to sensorService
  sensorService.addCharacteristic(humidityCharac);    // add the humidity sensor characteristic to sensorService
  BLE.addService(sensorService);                    // add the sensorService to this peripheral
  temperatureCharac.writeValue("00.0");    // set the initial value for the characeristic temperatureChar
  humidityCharac.writeValue("00.0");       // set the initial value for the characeristic humidityChar
  BLE.advertise();                        // start advertising this BLE peripheral to remote central devices

  dht.begin();                // call the begin class in the dht object
}

void loop() {
  
  BLEDevice central = BLE.central();      // wait for BLE central devices to connect

  if (central) {                          // run if a central is connected to sensor peripheral
    while (central.connected()) {         // while the central is still connected to fan peripheral
      long currentMillis = millis();      // current time to be compared with previousMillis
      
      if (currentMillis - previousMillis >= 2000) {    // if >2 seconds from last reading. DHT22 sampling rate is 0.5Hz
        float temp = dht.readTemperature();            // read temperature
        char tempString[5];
        dtostrf(temp, 4, 1, tempString);               // convert float to char array
        temperatureCharac.writeValue(tempString);   // write temperature to characteristic

        float hum = dht.readHumidity();                // read humidity
        char humString[5];
        dtostrf(hum, 4, 1, humString);                 // convert float to char array
        humidityCharac.writeValue(humString);       // write humidity to characteristic

        previousMillis = currentMillis;                // reset previousMillis
      }
    }
  }
}
