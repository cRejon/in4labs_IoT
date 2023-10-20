/*
  Sketch name: Master Blink - UART
  UNED Arduino IoT lab.

  Serial/UART Master that turns an LED from 'Slave Blink - UART' on for a set 
  delay time, then off, repeatedly. 

  The default blink time is 1 seconds.

  Circuit:
    - RX (pin 0) connected to Slave TX pin  
    - TX (pin 1) connected to Slave RX pin 
*/

#define BLINK_DELAY 1000        // time in ms that the led is on/off

void setup() {
  Serial1.begin(115200);        // initialize serial and wait for port to open:
  while (!Serial1) {
    ;                           // Serial used for communication with other boards
  }
}

void loop() {
  Serial1.write("turnLedOn");   // send instruction to slaves to turn led on
  Serial1.flush();
  delay(BLINK_DELAY);           // wait for set delay time 
    
  Serial1.write("turnLedOff");  // send instruction to slaves to turn led off
  Serial1.flush();
  delay(BLINK_DELAY);           // wait for set delay time   
}
