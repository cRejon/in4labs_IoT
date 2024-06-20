/*
  Sketch name: Temp Control LCD - UART
  UNED Arduino IoT lab.

  UART Temperature and Humidity control - LCD

  This example creates a UART Master that obtains sensor data from a 
  UART Slave (Temp Control Sensor - UART), presents the information in a 
  LCD screen and instructs an UART Slave (Temp Control Fan - UART) to 
  turn on/off a fan.

  Circuit (UART):
    * RX pin 0 slaves connected to TX pin from Master 1
    * TX pin 1 slaves connected to RX pin from Master 0
  
  Review example 'LCD Test' to see the functionalities of the LCD screen.
 */

#include <LiquidCrystal.h>      // include LCD library

#define TEMP_LIMIT 50.0    // if this threshold is reached - instruct fan on 
#define FAN_ON 0x01            // value to write to fan device to turn it on
#define FAN_OFF 0x00           // value to write to fan device to turn it off

byte fanStatus = FAN_OFF;           // fan status
byte previousFanStatus = FAN_OFF;   // previous fan status
char temperatureReading[5];               // char array to hold temperature reading
char humidityReading[5];                // char array to hold humidity reading

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial0.begin(115200);      // initialize serial and wait for port to open:
  while (!Serial0) {
    ;                         // Serial used for communication with other boards
  }

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

void loop() {

  Serial0.write("temperature");       // send request to obtain temperature sensor reading
  delay(100);                         // leave time for transmision to be received and replied generated
  // save temperature reading in temperatureReading char array
  int i = 0;                          // counter for number of bytes received
  while(Serial0.available()){         // slave may send less than requested
      char c = Serial0.read();          // receive a byte as character
      temperatureReading[i] =  c;                 // load received char into char array
      i++;
    if (i == 5){
      break;  
    }
  }

  Serial0.write("humidity");          // send request to obtain humidity sensor reading
  delay(100);                         // leave time for transmision to be received and replied generated
  // save humidity reading in humidityReading char array
  int j = 0;                          // counter for number of bytes received
  while(Serial0.available()){         // slave may send less than requested
      char c = Serial0.read();          // receive a byte as character
      humidityReading[j] =  c;                  // load received char into char array
      j++;
      if (j == 5){
          break;  
      }
  }  

  // convert temperatureReading to float and compare to TEMP_LIMIT
  float temperatureFloat = atof(temperatureReading);  // convert char array to float
  if (temperatureFloat > TEMP_LIMIT){  // if the threshold is reached 
    fanStatus = FAN_ON;           // activate fan
  } else {
    fanStatus = FAN_OFF;          
  }  

  if (fanStatus != previousFanStatus){ // if fan status has changed
    if (fanStatus == FAN_ON){                                      
      Serial0.write("turnFanOn");       // send instruction to slaves to turn fan on
      Serial0.flush();
    } else {                         
      Serial0.write("turnFanOff");      // send instruction to slaves to turn fan off
      Serial0.flush();
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
  lcd.print(temperatureReading);
  lcd.print(" C");

  lcd.setCursor(1, 1);
  lcd.print("H:");
  lcd.print(humidityReading);
  lcd.print(" %");

  lcd.setCursor(10, 0);
  lcd.print("|"); 
  lcd.setCursor(10, 1);
  lcd.print("|"); 
 
  lcd.setCursor(12, 0);
  lcd.print("Fan");
  lcd.setCursor(12, 1);
  lcd.print(fanStatus == FAN_ON ? "ON" : "OFF");
}