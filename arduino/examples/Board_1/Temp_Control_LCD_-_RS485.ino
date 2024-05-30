/*
  Sketch name: Temp Control LCD - RS485
  UNED Arduino IoT lab.

  RS485 Temperature and Humidity control - LCD

  This example creates a master that obtains sensor data from a 
  slave (Temp Control Sensor - RS485), presents the information in a 
  LCD screen and instructs slave (Temp Control Fan - RS485) to 
  turn on/off a fan.
  
  Communication is done via RS-485. Arduinos are conected via SoftwareSerial
  from pins 7 (RX_PIN) and 8 (TX_PIN) to modules MAX485 which are then connected
  to each other via an RS-485 bus.
  
  Circuit (RS485):
    - Sofware RX (pin 7) connected to MAX485 data reception RX
    - Sofware TX (pin 8) connected to MAX485 data transmission TX
      
  Review example 'LCD Test' to see the functionalities of the LCD screen.
 */

#include <LiquidCrystal.h>      // include LCD library

//HardwareSerial pins
#define TX_PIN 7                  
#define RX_PIN 8                

#define TEMP_LIMIT 50.0    // if this threshold is reached - instruct fan on 
#define ON 0x01            // value to write to fan device to turn it on
#define OFF 0x00           // value to write to fan device to turn it off

byte fanStatus = OFF;           // fan status
byte previousFanStatus = OFF;   // previous fan status
char tempRead[5];               // char array to hold temperature reading
char humRead[5];                // char array to hold humidity reading

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    delay(1000);          // wait for serial port to initialize
    Serial1.flush();      // clear serial port buffer

    lcd.begin(16, 2);           // set up the LCD's number of columns and rows
    lcd.clear();
}

void loop() {
    Serial1.write("temperature");       // send request to obtain temperature sensor reading
    delay(100);                         // leave time for transmision to be received and replied generated
    // save temperature reading in tempRead char array
    int i = 0;                          // counter for number of bytes received
    while(Serial1.available()){         // slave may send less than requested
        char c = Serial1.read();          // receive a byte as character
        tempRead[i] =  c;                 // load received char into char array
        i++;
        if (i == 5){
        break;  
        }
    }

    Serial1.write("humidity");          // send request to obtain humidity sensor reading
    delay(100);                         // leave time for transmision to be received and replied generated
    // save humidity reading in humRead char array
    int j = 0;                          // counter for number of bytes received
    while(Serial1.available()){         // slave may send less than requested
        char c = Serial1.read();          // receive a byte as character
        humRead[j] =  c;                  // load received char into char array
        j++;
        if (j == 5){
            break;  
        }
    }  

    // convert tempRead to float and compare to TEMP_LIMIT
    float temperatureFloat = atof(tempRead);  // convert char array to float
    if (temperatureFloat > TEMP_LIMIT){  // if the threshold is reached 
        fanStatus = ON;           // activate fan
    } else {
        fanStatus = OFF;          
    }  

    if (fanStatus != previousFanStatus){ // if fan status has changed
        if (fanStatus == ON){                                      
        Serial1.write("turnFanOn");       // send instruction to slaves to turn fan on
        Serial1.flush();
        } else {                         
        Serial1.write("turnFanOff");      // send instruction to slaves to turn fan off
        Serial1.flush();
        }
        previousFanStatus = fanStatus;      // update previous fan status
    }
    
    updateLCD();                     // update LCD screen with new sensor readings and fan status
    
    delay(2000);                     // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
}

// function that updates sensor readings and fan status in LCD screen
void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("T:");
    lcd.print(tempRead);
    lcd.print(" C");

    lcd.setCursor(1, 1);
    lcd.print("H:");
    lcd.print(humRead);
    lcd.print(" %");

    lcd.setCursor(10, 0);
    lcd.print("|"); 
    lcd.setCursor(10, 1);
    lcd.print("|"); 
    
    lcd.setCursor(12, 0);
    lcd.print("Fan");
    lcd.setCursor(12, 1);
    lcd.print(fanStatus == ON ? "ON" : "OFF");
}
