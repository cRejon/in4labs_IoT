/*
  Sketch name: Temp Control Sensor - UART
  UNED Arduino IoT lab. 

  UART Temperature and Humidity control - Sensor

  This example creates a UART Slave that provides sensor data to
  UART master (Temp Control LCD - UART) upon request.

  Circuit:
    - DHT22_PIN (pin 2) connected to sensor data line
    - RX (pin 0) in slaves connected to TX pin from Master (pin 1)
    - TX (pin 1) in slaves connected to RX pin from Master (pin 0)
     
  Review example 'DHT22 Sensor Test' for a detailed description of how this sensor works.
*/

#include <DHT.h> // include the DHT library

#define DHT22_PIN 2                 // define the type data pin
#define DHTType DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DHT22_PIN, DHTType);    // instantiate the dht class with our data pin and DHT type.

void setup() {
  Serial0.begin(115200);      // initialize serial and wait for port to open:
  while (!Serial0) {
    ;                         // Serial used for communication with other boards
  }

  dht.begin();                // call the begin class in the dht object
}


long previousMillis = 0;    // last time the sensor was checked, in ms
float temperature;     // make a float to hold data read from the sensor
float humidity;        // make a float to hold data read from the sensor

void loop() {
  
  long currentMillis = millis();              // current time to be compared with previousMillis
  
  if(currentMillis-previousMillis>= 2000){    // if >2 seconds from last reading. DHT22 sampling rate is 0.5Hz
    
    temperature = dht.readTemperature();  // read temperature
    humidity = dht.readHumidity();        // read humidity
    previousMillis=currentMillis;             // reset previousMillis
  }

  if(Serial0.available()){              // run code only if data received from master

    delay(1);                           // allow time for message to be received in full
    
    String masterRequest = "";          // accumulate char bytes of data from master in String
    while (Serial0.available()){          
      char c = Serial0.read();
      masterRequest += c;
    }

    char sensorChar[5];                 // char array that will be used to send sensor data to master in bytes
  
    if (masterRequest == "temperature"){          // if request for temperature
      String temperatureRead = String(temperature, 1);  // convert data to String
      temperatureRead.toCharArray(sensorChar, 5);  // String sensor reading to char array
      Serial0.write(sensorChar, 4);                // send char array to master

    } else if (masterRequest == "humidity"){      // if request for humidity
      String humidityRead = String(humidity, 1);  // convert data to String
      humidityRead.toCharArray(sensorChar, 5);     // String sensor reading to char array
      Serial0.write(sensorChar, 4);                // send char array to master
    }
  }
}
