/*
  Sketch name: Slave Blink - RS485
  UNED Arduino IoT lab.

  Slave that turns, under commands from 'Master Blink - RS485' example, 
  a LED on for a set delay time, then off, repeatedly.

  Communication is done via RS-485. Arduinos are conected via SoftwareSerial
  from pins 7 (RX) and 8 (TX) to modules MAX485 which are then connected
  to each other via an RS-485 bus.

  Circuit:
    - RGB LED connected to pins 5 (red), 6 (green) and 9 (blue)
    - Sofware RX (pin 7) connected to MAX485 RO (Receiver Out)
    - Sofware TX (pin 8) connected to MAX485 DI (Driver In)
    - Pin 3 connected to MAX485 DE and RE. When set HIGH module is sending data.
      When is set LOW, module is receiving data.
*/

#include <SoftwareSerial.h>

#define RGB_PIN 5 
//SoftwareSerial pins
#define RX_PIN 7                  // connected to RO (Receiver Out)
#define TX_PIN 8                  // connected to DI (Driver In)
#define TRANSMIT_ENABLER 3        // HIGH to send data, LOW to receive

SoftwareSerial RS485 (RX_PIN, TX_PIN);  // assign name, RX and TX pins to software serial           

void setup() {
  RS485.begin(9600);                    // initialize serial and wait for port to open
  while (!RS485) {
    ;                                   // Serial used for communication with other boards
  }

  pinMode(TRANSMIT_ENABLER, OUTPUT);    // output signal to control MAX485 mode
  digitalWrite(TRANSMIT_ENABLER, LOW);  // setup as slave. MAX485 receiving data
  
  pinMode(RGB_PIN, OUTPUT);             // initialize digital pin RGB_PIN as an output.
  digitalWrite(RGB_PIN, LOW);           // start with the LED off 
}

void loop() {
  if(RS485.available()){                // run code only if data received from master
    delay(10);                          // allow time for message to be received in full
    
    String masterRequest = "";          // accumulate char bytes of data from master in String
    while (RS485.available()){          
      char c = RS485.read();
      masterRequest += c;
    }

    if (masterRequest == "turnLedOn"){
      digitalWrite(RGB_PIN, HIGH);       // turn the LED on 
    } else 
    if (masterRequest == "turnLedOff"){
      digitalWrite(RGB_PIN, LOW);        // turn the LED off
    }
  }
}
