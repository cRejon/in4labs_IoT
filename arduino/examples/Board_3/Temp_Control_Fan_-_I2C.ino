/*
  Sketch name: Temp Control Fan - I2C
  UNED Arduino IoT lab. 

  I2C Temperature and Humidity control - Fan

  This example creates a slave that controls a FAN under the commands
  received from master programmed with 'Temp Control LCD - I2C' example.

  Communication is done via I2C using Wire.h library. 
  Arduinos are conected to I2C BUS through dedicated pins SCL (Serial Clock) 
  and SDA (Serial Data). This slave is contacted by master in address 2.

  Circuit:
    - Fan (+) conected to 12 V (+)
    - Fan (-) conected to Relay NC (Normally Closed) 
    - Relay COM (common) to ground
    - Pin 2 connected to Relay Signal pin
    - SCL (Serial Clock) and SDA (Serial Data) to I2C bus
  
  Review example 'Fan Test' for a detailed description of how this fan works.
*/

#include <Wire.h>             // include I2C library

#define FAN_PIN 2             // define the pin connected to Relay

void setup() {
  Wire.begin(2);                    // join i2c as slave bus with address #2
  Wire.onReceive(receiveEvent);     // if data received via I2C bus, run receiveEvent function

  pinMode(FAN_PIN, OUTPUT);         // set the FAN pin as output
  digitalWrite(FAN_PIN, LOW);      // fan off
}

void loop() {
  while(1);                         // wait for communication from master
}

void receiveEvent(int numBytes){      // function run if data received from master
  
  byte c;                             // byte for data received from master

  while(Wire.available()){            // while data available from master (should only be one byte)
    c = Wire.read();                  // read byte sent by Master
  }

  switch (c){                         // select case depending on byte sent by master
    case 0:
      digitalWrite(FAN_PIN, LOW);    // turn the FAN off   
    break;
    case 1:
      digitalWrite(FAN_PIN, HIGH);     // turn the FAN on 
    break;
  }
}
