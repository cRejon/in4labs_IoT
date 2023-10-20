/*
  Sketch name: Temp Control Fan - UART
  UNED Arduino IoT lab. 

  UART Temperature and Humidity control - Fan

  This example creates a UART slave that controls a FAN under the commands
  received from UART master programmed with 'Temp Control LCD - UART'.

  Circuit:
    - Fan (+) conected to 12 V (+)
    - Fan (-) conected to Relay NC (Normally Closed) 
    - Relay COM (common) to ground
    - Pin 2 connected to Relay Signal pin
    - RX (pin 0) in slaves connected to TX pin from Master (pin 1)
    - TX (pin 1) in slaves connected to RX pin from Master (pin 0)
  
  Review example 'Fan Test' for a detailed description of how this fan works.
*/

#define FAN_PIN 2             // define the pin connected to Relay

void setup() {
  Serial1.begin(115200);      // initialize serial and wait for port to open:
  while (!Serial1) {
    ;                         // Serial used for communication with other boards
  }

  pinMode(1, INPUT); // TX (data out) pin set as input. Default output mode will conflict with
                    // data sent from 'Temp Control Sensor - UART', because they share the same bus

  pinMode(FAN_PIN, OUTPUT);         // set the FAN pin as output
  digitalWrite(FAN_PIN, HIGH);      // fan off
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
      digitalWrite(FAN_PIN, LOW);               // turn the FAN on
    } else if (masterRequest == "turnFanOff"){
      digitalWrite(FAN_PIN, HIGH);              // turn the FAN off
    }
  }
}
