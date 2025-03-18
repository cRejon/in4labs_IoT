/*
  Sketch name: Temp Control Sensor - I2C
  UNED Arduino IoT lab. 

  I2C Temperature and Humidity control - Sensor

  This example creates a slave that provides sensor data to
  master (Temp Control LCD - I2C) upon request.

  Communication is done via I2C using Wire.h library. 
  Arduinos are conected to I2C BUS through dedicated pins SCL (Serial Clock) 
  and SDA (Serial Data). This slave is contacted by master in address 9.

  Circuit:
    - DHT22_PIN (pin 2) connected to sensor data line
    - SCL (Serial Clock) and SDA (Serial Data) to I2C bus.
 
  Review example 'DHT22 Sensor Test' for a detailed description of how this sensor works.
*/

#include <Wire.h>            // include I2C library
#include <DHT.h>             // include the DHT library

#define DHT22_PIN 2                 // define the type data pin
#define DHTType DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DHT22_PIN, DHTType);    // instantiate the dht class with our data pin and DHT type.

char sensorChar[9];                // char array that will be used to send sensor data to master in bytes

void setup() {
  Wire.begin(9);                      // join i2c as slave bus with address #9
  Wire.onRequest(requestEvent);       // if request received via I2C bus, run requestEvent function

  dht.begin();                // call the begin class in the dht object
}


void loop() {
  float temperature = dht.readTemperature();  // read temperature as Celsius
  float humidity = dht.readHumidity();        // read humidity as percentage
  
  if (!isnan(temperature) && !isnan(humidity)) {   // if data received from sensor is valid
    String sensorData = String(temperature, 1) + String(humidity, 1);  // convert data to String
    sensorData.toCharArray(sensorChar, 9);       // convert String to char array
  }

  delay(2000);         // delay 2 seconds. DHT22 sampling rate is 0.5Hz
}

void requestEvent(){      // function run if request received from master
  Wire.write(sensorChar); // respond with mesage of 9 bytes (Only 8 bytes with information)
}
