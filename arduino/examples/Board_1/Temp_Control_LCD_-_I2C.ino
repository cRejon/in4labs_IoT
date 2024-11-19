/*
  Sketch name: Temp Control LCD - I2C
  UNED Arduino IoT lab. 

  I2C Temperature and Humidity control - LCD

  This example creates a master that obtains sensor data from a 
  slave (Temp Control Sensor - I2C), presents the information in a 
  LCD screen and instructs slave (Temp Control Fan - I2C) to 
  turn on/off a fan.

  Communication is done via I2C using Wire.h library. 
  Arduinos are conected to I2C BUS through dedicated pins SCL (Serial Clock) 
  and SDA (Serial Data). Master devices don´t require an I2C address.

  Circuit (I2C):
    - SCL (Serial Clock) and SDA (Serial Data) to I2C bus.
 
  Review example 'LCD Test' to see the functionalities of the LCD screen.
*/

#include <Wire.h>               // include I2C library
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
  Wire.begin();               // join i2c bus as master. no address required

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear();   
}

void loop() {  
  char sensorChar[9] = {0,0,0,0,0,0,0,0,0}; // clear chart array for incoming data from sensor

  // obtain sensor reading
  Wire.requestFrom(9, 8);             // send request for 8 bytes from slave device in address #9 (sensor)

  int i = 0;                // counter for number of bytes received
  while(Wire.available()){  // slave may send less than requested
    char c = Wire.read();   // receive a byte as character
    sensorChar[i] =  c;     // load received char into char array
    i++;
    if (i == 8){
      break;  
    }
  }

  for (int i = 0; i < 4; i++){        // parse temperature reading (first 4 bytes, xx.x)
    temperatureReading[i] = sensorChar[i];
  }

  for (int i = 4; i < 8; i++){        // parse humidity reading (last 4 bytes, yy.y)
    humidityReading[i-4] = sensorChar[i]; 
  }

  // convert temperatureReading to float and compare to TEMP_LIMIT
  float temperatureFloat = atof(temperatureReading);  // convert char array to float
  if (temperatureFloat > TEMP_LIMIT){  // if the threshold is reached 
    fanStatus = FAN_ON;           // activate fan
  } else {
    fanStatus = FAN_OFF;          
  }  

  if (fanStatus != previousFanStatus){  // if fan status has changed
    Wire.beginTransmission(2);    // transmission to slave device #2 (fan)
    Wire.write(fanStatus);        // write fan status
    Wire.endTransmission(2);      // end transmission to slave device #2 (fan)
    previousFanStatus = fanStatus;  // update previous fan status
  }
  
  updateLCD();                    // update LCD screen with new sensor readings and fan status
  
  delay(2000);                    // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
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

