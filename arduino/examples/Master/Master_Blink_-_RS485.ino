/*
  Sketch name: Master Blink - RS485
  UNED Arduino IoT lab.

  RS485 Master that turns an LED from 'Slave Blink - RS485' on for a set 
  delay time, then off, repeatedly. 

  The default blink time is 1 seconds.

  Communication is done via RS-485. Arduinos are conected via SoftwareSerial
  from pins 7 (RX) and 8 (TX) to modules MAX485 which are then connected
  to each other via an RS-485 bus.

  Circuit:
    - Sofware RX (pin 7) connected to MAX485 RO (Receiver Out)
    - Sofware TX (pin 8) connected to MAX485 DI (Driver In)
    - Pin 3 connected to MAX485 DE and RE. When set HIGH module is sending data.
      When is set LOW, module is receiving data.
*/

#include <SoftwareSerial.h>

#define BLINK_DELAY 1000          // time in ms that the LED is on/off

//SoftwareSerial pins
#define RX_PIN 7                  // connected to RO (Receiver Out)
#define TX_PIN 8                  // connected to DI (Driver In)
#define TRANSMIT_ENABLER 3        // HIGH to send data, LOW to receive

SoftwareSerial RS485 (RX_PIN, TX_PIN);  // assign name, RX and TX pins to software serial

void setup() {
  RS485.begin(9600);                    // initialize serial and wait for port to open:
  while (!RS485) {
    ;                                   // Serial used for communication with other boards
  }

  pinMode(TRANSMIT_ENABLER, OUTPUT);
  digitalWrite(TRANSMIT_ENABLER, HIGH); //setup as master
}

void loop() {
  RS485.write("turnLedOn");      // send instruction to slaves to turn LED on
  RS485.flush();
  delay(BLINK_DELAY);            // wait for set delay time 
    
  RS485.write("turnLedOff");     // send instruction to slaves to turn LED off
  RS485.flush();
  delay(BLINK_DELAY);            // wait for set delay time   
}
