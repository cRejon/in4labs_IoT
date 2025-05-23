/*
    Sketch name: Slave Blink - RS485
    UNED Arduino IoT lab.

    Slave that turns, under commands from 'Master Blink - RS485' example, 
    a LED on for a set delay time, then off, repeatedly.

    Communication is done via RS-485. Arduinos are connected via HardwareSerial
    to the RS-485 module.

    Circuit:
        - RGB LED connected to pins A0 (red), A1 (green) and A2 (blue)
        - HardwareSerial connected to RS-485 module
*/

#define RGB_PIN A0
//SoftwareSerial pins
#define TX_PIN 7                  
#define RX_PIN 8  

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port

    pinMode(RGB_PIN, OUTPUT);             // initialize digital pin RGB_PIN as an output.
    digitalWrite(RGB_PIN, LOW);           // start with the LED off 
}
void loop() {
    if(Serial1.available()){                // run code only if data received from master
        delay(10);                          // allow time for message to be received in full
        
        String masterRequest = "";          // accumulate char bytes of data from master in String
        while (Serial1.available()){          
            char c = Serial1.read();
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
