/*
    Sketch name: Master Blink - RS485
    UNED Arduino IoT lab.

    RS485 Master that turns an LED from 'Slave Blink - RS485' on for a set 
    delay time, then off, repeatedly. 

    The default blink time is 1 second.

    Communication is done via RS-485. Arduinos are connected via hardware serial
    ports on the Arduino Nano ESP32.

    Circuit:
        - Hardware RX (Serial1) connected to MAX485 RO (Receiver Out)
        - Hardware TX (Serial1) connected to MAX485 DI (Driver In)
*/

#define BLINK_DELAY 1000          // time in ms that the LED is on/off
//SoftwareSerial pins
#define TX_PIN 7                  
#define RX_PIN 8                  

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    delay(1000);          // wait for serial port to initialize
    Serial1.flush();      // clear serial port buffer
}

void loop() {
    Serial1.write("turnLedOn");      // send instruction to slaves to turn LED on
    Serial1.flush();
    delay(BLINK_DELAY);            // wait for set delay time 

    Serial1.write("turnLedOff");     // send instruction to slaves to turn LED off
    Serial1.flush();
    delay(BLINK_DELAY);            // wait for set delay time   
}
