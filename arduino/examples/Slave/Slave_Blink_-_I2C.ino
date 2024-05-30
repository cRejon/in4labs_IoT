/*
  Sketch name: Slave Blink - I2C
  UNED Arduino IoT lab. 

  Slave that turns, under commands from 'Master Blink - I2C' example, 
  a LED on for a set delay time, then off, repeatedly.

  Communication is done via I2C using 'Wire.h' library. 
  Arduinos are conected to I2C BUS through dedicated pins SCL (Serial Clock) 
  and SDA (Serial Data). This slave is contacted by Master in address 2.

  Circuit:
    - RGB LED connected to pins A0 (red), A1 (green) and A2 (blue)
    - SCL (Serial Clock) and SDA (Serial Data) to I2C bus
*/

#include <Wire.h> // include I2C library

#define RGB_PIN A0                  


void setup() {
  Wire.begin(2);                      // join i2c as slave bus with address #2
  Wire.onReceive(receiveEvent);       // if data received via I2C bus, run receiveEvent function

  pinMode(RGB_PIN, OUTPUT);           // initialize digital pin RGB_PIN as an output
  digitalWrite(RGB_PIN, LOW);         // start with the LED off by making the voltage LOW
}

void loop() {
  while(1);                           // wait for communication from Master
}

void receiveEvent(int numBytes){      // function run if data received from Master
  
  byte c;                             // byte for data received from Master

  while(Wire.available()){            // while data available from Master (should only be one byte)
    c = Wire.read();                  // read byte sent by Master
  }

  switch (c){                         // select case depending on byte sent by Master
    case 0:
      digitalWrite(RGB_PIN, LOW);     // turn the LED off 
    break;
    case 1:
      digitalWrite(RGB_PIN, HIGH);    // turn the LED on 
    break;
  }
}
