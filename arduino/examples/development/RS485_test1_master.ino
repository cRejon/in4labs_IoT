/*
RS485
18/10/19
*/
#include <LiquidCrystal.h>      // include LCD library
#include<SoftwareSerial.h> 

#define MAX_PIN 3  //MAX485 control pin

SoftwareSerial RS485(7, 8);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String id_sensor;
char type;
char status_device;

void setup(){
  RS485.begin(9600);
  RS485.setTimeout (100);

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear();
  lcd.setCursor(0, 0);

  pinMode(MAX_PIN, OUTPUT);
  digitalWrite(MAX_PIN, LOW);  //Disable max485 MAX_PIN
}

void loop(){
  SendRequest();
  delay(2000);
}
  

void CheckMsg(){
  while(RS485.available() == 0){
    delay(10);
  }

  if(RS485.available() > 0){
    String slaveRequest = "";     // accumulate char bytes of data from Master in String
    while (RS485.available()){          
      char c = RS485.read();
      slaveRequest += c;
    }
    //Serial.println(received); 
    lcd.print("byteReceived");
  }
}

void SendRequest(){
  digitalWrite(MAX_PIN, HIGH);  //Enable max485 MAX_PIN
  RS485.write("001");
  RS485.flush();
  digitalWrite(MAX_PIN,LOW);
  CheckMsg();
}