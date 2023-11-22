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
    - Sofware RX (pin 7) connected to MAX485 RO (Receiver Out)
    - Sofware TX (pin 8) connected to MAX485 DI (Driver In)
    - Pin 3 connected to MAX485 DE and RE. When set HIGH module is sending data.
      When is set LOW, module is receiving data.
     
  Review example 'DHT22 Sensor Test' for a detailed description of how this sensor works.
 */

#include <DHT.h>  // include the DHT library.
#include <SoftwareSerial.h>

#define DHT22_PIN 2                  // define the type data pin
//SoftwareSerial pins
#define RX_PIN 7                       // connected to RO (Receiver Out)
#define TX_PIN 8                       // connected to DI (Driver In)
#define TRANSMIT_ENABLER 3             // high to send data. low to receive.


DHT dht = DHT(DHT22_PIN, DHT22);    // instantiate the dht class with our data pin and DHT type.

SoftwareSerial RS485(RX_PIN, TX_PIN); // assign name, RX and TX pins to software serial

void setup() {
  RS485.begin(9600);          // initialize serial and wait for port to open:
  while (!RS485) {
    ;                         // Serial used for communication with other boards
  }

  dht.begin();                // call the begin class in the dht object

  pinMode(TRANSMIT_ENABLER, OUTPUT);    // output signal to control MAX485 mode.
  digitalWrite(TRANSMIT_ENABLER, LOW);  // setup as slave. MAX485 receiving data
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

  if(RS485.available()){               // run code only if data received from master
    delay(10);                         // allow time for message to be received in full
    
    String masterRequest = "";         // accumulate char bytes of data from master in String
    while (RS485.available()){          
      char c = RS485.read();
      masterRequest += c;
    }

    char sensorChar[6];                  // char array that will be used to send sensor data to master in bytes
  
    if (masterRequest == "temperature"){           // if request for temperature
      digitalWrite(TRANSMIT_ENABLER, HIGH);        // enable transmittion. MAX485 sending data
      String temperatureRead = String(temperature, 2);  // convert data to String
      temperatureRead.toCharArray(sensorChar,6);   // String sensor reading to char array
      RS485.write(sensorChar,5);                   // send char array to master;
      RS485.flush();
      digitalWrite(TRANSMIT_ENABLER, LOW);         // disable transmittion. MAX485 receiving data
      
    } else if (masterRequest == "humidity"){       // if request for temperature
      digitalWrite(TRANSMIT_ENABLER, HIGH);        // enable transmittion. MAX485 sending data
      String humidityRead = String(humidity, 2);  // convert data to String
      humidityRead.toCharArray(sensorChar,6);      // String sensor reading to char array
      RS485.write(sensorChar,5);                   // send char array to master;
      RS485.flush();
      digitalWrite(TRANSMIT_ENABLER, LOW);         // disable transmittion. MAX485 receiving data  
    }
  }  
}
