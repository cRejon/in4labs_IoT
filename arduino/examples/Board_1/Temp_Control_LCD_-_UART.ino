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
#define HUM_LIMIT 80.0     // if this threshold is reached - instruct fan on 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String fanStatus = "OFF";             // fan status to be printed by LCD screen
String temperatureRead;               // make a String to hold data received from the sensor
String humidityRead;                  // make a String to hold data received from the sensor

void setup() {
  Serial1.begin(115200);      // initialize serial and wait for port to open:
  while (!Serial1) {
    ;                         // Serial used for communication with other boards
  }

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

void loop() {
  float temperatureFloat;     // make a float to convert String data and compare to TEMP_LIMIT
  float humidityFloat;        // make a float to convert String data and compare to HUM_LIMIT
  temperatureRead = "";       // clear data for new reading
  humidityRead = "";          // clear data for new reading 


  Serial1.write("temperature");       // send request to obtain temperature sensor reading
  delay(100);                         // leave time for transmision to be received and replied generated
  if (Serial1.available()){             // if feedback from sensor is received
    String temp = Serial1.readString();    // read sensor data from serial
    temperatureRead = temp;
  }     
  
  Serial1.write("humidity");          // send request to obtain humidity sensor reading
  delay(100);                         // leave time for transmision to be received and replied generated
  if (Serial1.available()){             // if feedback from sensor is received
    String hum = Serial1.readString();    // read sensor data from serial
    humidityRead = hum;
  }

  updateLCD();                        // dispaly sensor readings in LCD screen

  temperatureFloat = temperatureRead.toFloat(); // convert String data and compare to TEMP_LIMIT
  humidityFloat = humidityRead.toFloat();       // convert String data and compare to HUM_LIMIT

  if (humidityFloat > HUM_LIMIT || temperatureFloat > TEMP_LIMIT){ // if any threshold is reached                            
    Serial1.write("turnFanOn");       // send instruction to slaves to turn fan on
    Serial1.flush();
    fanStatus = "ON";                 // update fan status to be printed by LCD screen
    
  } else {                         // if no threshold is reached
    Serial1.write("turnFanOff");      // send instruction to slaves to turn fan off
    Serial1.flush();
    fanStatus = "OFF";                // update fan status to be printed by LCD screen
  }
  
  updateLCD();                     // dispaly new fan status in LCD screen
  
  delay(2000);                     // wait 2 seconds before requesting new reading. Sensor sampling rate is 0.5Hz 
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