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
BLEService sensorService("181A");    // BLE Sensor Service

BLEIntCharacteristic tempCharacteristic("2A6E", BLERead);  // BLE Temperature Sensor Characteristic
BLEIntCharacteristic humCharacteristic("2A6F", BLERead);   // BLE Humidity Sensor Characteristic
     

void setup() {
 
  Serial.begin(9600);
  while (!Serial);

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
  BLE.poll();   // poll for BLE events

  float temp = dht.readTemperature();            
  int tempInt = (int) (temp*10);
  //Serial.println(tempInt);
  tempCharacteristic.writeValue(tempInt);   // update the temperature characteristic
  
  float hum = dht.readHumidity(); 
  int humInt = (int) (hum*10);
  //Serial.println(humInt);
  humCharacteristic.writeValue(humInt);     // update the humidity characteristic

  delay(2000);
}