/*
  Sketch name: Master Blink - I2C
  UNED Arduino IoT lab.

  Master that turns an LED from 'Slave Blink - I2C' on for a set 
  delay time, then off, repeatedly. 

  The default blink time is 1 seconds.
  
  Communication is done via I2C using 'Wire.h' library. 
  Arduinos are conected to I2C BUS through dedicated pins SCL (Serial Clock) 
  and SDA (Serial Data). Master devices don´t require an I2C address.

  Circuit:
    - SCL (Serial Clock) and SDA (Serial Data) to I2C bus
*/

#include <Wire.h> // include I2C library

#define BLINK_DELAY 1000        // time in ms that the LED is on/off


void setup() {
  Wire.begin();                 // join i2c bus as master. no address required
}

void loop() {
  Wire.beginTransmission(2);    // transmission to slave device #2 
  Wire.write(0x01);             // byte = 1; LED on
  Wire.endTransmission(2);      // end transmission to slave device #2
  delay(BLINK_DELAY);           // wait for set delay time 
    
  Wire.beginTransmission(2);    // transmission to slave device #2
  Wire.write(0x00);             // byte = 0; LED off
  Wire.endTransmission(2);      // end transmission to slave device #2 (LED)
  delay(BLINK_DELAY);           // wait for set delay time   
}
