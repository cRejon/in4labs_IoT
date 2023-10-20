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
#define HUM_LIMIT 80.0     // if this threshold is reached - instruct fan on  

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String fanStatus = "OFF";             // fan status to be printed by LCD screen
String temperatureRead;               // make a String to hold data received from the sensor
String humidityRead;                  // make a String to hold data received from the sensor

void setup() {
  Wire.begin();               // join i2c bus as master. no address required

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear();   
}

void loop() {
  float temperatureFloat;     // make a float to convert String data and compare to TEMP_LIMIT
  float humidityFloat;        // make a float to convert String data and compare to HUM_LIMIT
  temperatureRead = "";       // clear data for new reading
  humidityRead = "";          // clear data for new reading 
  
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
    temperatureRead += sensorChar[i]; 
  }

  for (int i = 4; i < 8; i++){       // parse humidity reading (last 4 bytes, yy.y)
    humidityRead += sensorChar[i]; 
  }
  
  updateLCD();                        // dispaly sensor readings in LCD screen

  temperatureFloat = temperatureRead.toFloat(); // convert String data and compare to TEMP_LIMIT
  humidityFloat = humidityRead.toFloat();       // convert String data and compare to HUM_LIMIT
  
  if (humidityFloat > HUM_LIMIT || temperatureFloat > TEMP_LIMIT){  // if any threshold is reached                             
    Wire.beginTransmission(2);    // transmission to slave device #2 (fan)
    Wire.write(0x01);             // byte = 1; fan on
    Wire.endTransmission(2);      // end transmission to slave device #2 (fan)

    fanStatus = "ON";             // update fan status to be printed by LCD screen

  } else {                        // if no threshold is reached
    Wire.beginTransmission(2);    // transmission to slave device #2 (fan)
    Wire.write(0x00);             // byte = 0; fan off
    Wire.endTransmission(2);      // end transmission to slave device #2 (fan)
    
    fanStatus = "OFF";            // update fan status to be printed by LCD screen
  }
  
  updateLCD();                    // dispaly new fan status in LCD screen
  
  delay(2000);                    // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
}

// function that updates sensor readings and fan status in LCD screen
void updateLCD() {
  // clear LCD screen
  lcd.clear();  
  // sensor readings
  lcd.setCursor(1, 0);
  lcd.print("T:");
  lcd.setCursor(3, 0);
  lcd.print(temperatureRead);
  lcd.setCursor(7, 0);
  lcd.print(" C");
  
  lcd.setCursor(1, 1);
  lcd.print("H:");
  lcd.setCursor(3, 1);
  lcd.print(humidityRead);
  lcd.setCursor(7, 1);
  lcd.print(" %");
 
  // separation line
  lcd.setCursor(10, 0);
  lcd.print("|"); 
  lcd.setCursor(10, 1);
  lcd.print("|"); 
 
  // fan status
  lcd.setCursor(12, 0);
  lcd.print("Fan");
  lcd.setCursor(12, 1);
  lcd.print(fanStatus);
}

