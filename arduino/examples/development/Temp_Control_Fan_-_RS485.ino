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
    - Sofware TX (pin 7) connected to MAX485 data transmission TX
    - Sofware RX (pin 8) connected to MAX485 data reception RX

  Review example 'Fan Test' for a detailed description of how this fan works.
*/

#define FAN_PIN 2                 // define the pin connected to fan
//HardwareSerial pins
#define TX_PIN 7                  
#define RX_PIN 8   


void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    delay(1000);          // wait for serial port to initialize
    Serial1.flush();      // clear serial port buffer
   
    pinMode(1, INPUT); // TX (data out) pin set as input. Default output mode will conflict with
                    // data sent from 'Temp Control Sensor - UART', because they share the same bus

    pinMode(FAN_PIN, OUTPUT);               // set the FAN pin as an output
    digitalWrite(FAN_PIN, LOW);            // fan off
}

void loop() {
  
  if(Serial1.available()){              // run code only if data received from master
    
    delay(1);                           // allow time for message to be received in full
    
    String masterRequest = "";          // accumulate char bytes of data from master in String
    while (Serial1.available()){          
      char c = Serial1.read();
      masterRequest += c;
    }

    if (masterRequest == "turnFanOn"){
      digitalWrite(FAN_PIN, HIGH);               // turn the FAN on
    } else if (masterRequest == "turnFanOff"){
      digitalWrite(FAN_PIN, LOW);              // turn the FAN off
    }
  }
}
