/*
  Sketch name: Temp Control Fan - RS485
  UNED Arduino IoT lab.

  RS485 Temperature and Humidity control - Fan

  This example creates a slave that controls a FAN under the commands
  received from master programmed in 'Temp Control LCD - RS485'.
  
  Communication is done via RS-485. Arduinos are conected via SoftwareSerial
  from pins 7 (RX_PIN) and 8 (TX_PIN) to modules MAX485 which are then connected
  to each other via an RS-485 bus.
  
  Circuit:
    - Fan (+) conected to 12 V (+)
    - Fan (-) conected to Relay NC (Normally Closed) 
    - Relay COM (common) to ground
    - Pin 2 connected to Relay Signal pin
    - Sofware RX (pin 7) connected to MAX485 RO (Receiver Out)
    - Sofware TX (pin 8) connected to MAX485 DI (Driver In)
    - Pin 3 connected to MAX485 DE and RE. When set HIGH module is sending data.
      When is set LOW, module is receiving data.
  
  Review example 'Fan Test' for a detailed description of how this fan works.
*/

#include <SoftwareSerial.h>

#define FAN_PIN 2                 // define the pin connected to fan
//SoftwareSerial pins
#define RX_PIN 7                  // connected to RO (Receiver Out)
#define TX_PIN 8                  // connected to DI (Driver In)
#define TRANSMIT_ENABLER 3        // high to send data. low to receive


SoftwareSerial RS485 (RX_PIN, TX_PIN);     // assign name, RX and TX pins to software serial

void setup() {
  RS485.begin(9600);                       // initialize serial and wait for port to open:
  while (!RS485) {
    ; // Serial used for communication with other boards
  }
  
  pinMode(FAN_PIN, OUTPUT);               // set the FAN pin as an output
  digitalWrite(FAN_PIN, HIGH);            // fan off

  pinMode(TRANSMIT_ENABLER, OUTPUT);      // output signal to control MAX485 mode
  digitalWrite(TRANSMIT_ENABLER, LOW);    // setup as slave. MAX485 receiving data
}

void loop() {
  if(RS485.available()){                  // run code only if data received from master
    delay(10);                            //allow time for message to be received in full
    
    String masterRequest = "";            // accumulate char bytes of data from master in String
    while (RS485.available()){          
      char c = RS485.read();
      masterRequest += c;
    }

    if (masterRequest == "turnFanOn"){
      digitalWrite(FAN_PIN, LOW);                // turn the FAN on
    } else if (masterRequest == "turnFanOff"){
      digitalWrite(FAN_PIN, HIGH);               // turn the FAN off
    }
  }
}
