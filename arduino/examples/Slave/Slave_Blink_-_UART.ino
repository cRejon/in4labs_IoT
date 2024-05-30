/*
  Sketch name: Slave Blink - UART
  UNED Arduino IoT lab.

  UART Slave that turns, under commands from 'Master Blink - UART', 
  a LED on for a set delay time, then off, repeatedly.

  Circuit:
    - RGB LED connected to pins A0 (red), A1 (green) and A2 (blue)
    - RX (pin 0) connected to Master TX pin  
    - TX (pin 1) connected to Master RX pin  
*/

#define RGB_PIN A0               

void setup() {
  Serial1.begin(115200);           // initialize UART at 115200 bauds
  while (!Serial1) {
    ;                              // Serial used for communication with other boards
  }
  
  pinMode(RGB_PIN, OUTPUT);        // initialize digital pin RGB_PIN as an output.
  digitalWrite(RGB_PIN, LOW);      // start with the LED off by making the voltage HIGH
}

void loop() {
  if(Serial1.available()){         // run code only if data received from Master
    delay(1);                      // allow time for message to be received in full
    
    String masterRequest = "";     // accumulate char bytes of data from Master in String
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
