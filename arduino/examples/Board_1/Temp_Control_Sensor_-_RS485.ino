/*
  Sketch name: Temp Control Sensor - RS485
  UNED Arduino IoT lab.

  RS485 Temperature and Humidity control - Sensor

  This example creates a slave that provides sensor data to
  Master (Temp Control LCD - RS485) upon request.

  Communication is done via RS-485. Arduinos are conected via SoftwareSerial
  from pins 7 (RX_PIN) and 8 (TX_PIN) to modules MAX485 which are then connected
  to each other via an RS-485 bus.
  
  Circuit:
    - DHT22_PIN (pin 2) connected to sensor data line.
    - Sofware TX (pin 7) connected to MAX485 data transmission TX
    - Sofware RX (pin 8) connected to MAX485 data reception RX
     
  Review example 'DHT22 Sensor Test' for a detailed description of how this sensor works.
 */

#include <DHT.h>  // include the DHT library.

#define DHT22_PIN 2                  // define the type data pin
//HardwareSerial pins
#define TX_PIN 7                  
#define RX_PIN 8     

DHT dht = DHT(DHT22_PIN, DHT22);    // instantiate the dht class with our data pin and DHT type.

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    while (!Serial1) {
      ;                         // wait for serial port to initialize
    }         
    Serial1.flush();      // clear serial port buffer

    dht.begin();                // call the begin class in the dht object
}

long previousMillis = 0;    // last time the sensor was checked, in ms
float temperature;     // make a String to hold data read from the sensor
float humidity;        // make a String to hold data read from the sensor

void loop() {
  
  long currentMillis = millis();              // current time to be compared with previousMillis
  
  if(currentMillis-previousMillis>= 2000){    // if >2 seconds from last reading. DHT22 sampling rate is 0.5Hz
    
    temperature = dht.readTemperature();  // read temperature
    humidity = dht.readHumidity();        // read humidity
    previousMillis=currentMillis;             // reset previousMillis
  }

  if(Serial1.available()){              // run code only if data received from master

    delay(10);                           // allow time for message to be received in full
    
    String masterRequest = "";          // accumulate char bytes of data from master in String
    while (Serial1.available()){          
      char c = Serial1.read();
      masterRequest += c;
    }

    char sensorChar[5];                 // char array that will be used to send sensor data to master in bytes
  
    if (masterRequest == "temperature"){          // if request for temperature
      String temperatureRead = String(temperature, 1);  // convert data to String
      temperatureRead.toCharArray(sensorChar, 5);  // String sensor reading to char array
      Serial1.write(sensorChar, 4);                // send char array to master

    } else if (masterRequest == "humidity"){      // if request for humidity
      String humidityRead = String(humidity, 1);  // convert data to String
      humidityRead.toCharArray(sensorChar, 5);     // String sensor reading to char array
      Serial1.write(sensorChar, 4);                // send char array to master
    }
  }
}
