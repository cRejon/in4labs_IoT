#include <SoftwareSerial.h>
#include <LiquidCrystal.h>      // include LCD library
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        7  //Serial Receive pin
#define SSerialTX        8  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*-----( Declare Variables )-----*/
int byteReceived;
int byteSend;

void setup()   
{
  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear();
  lcd.setCursor(0, 0);

  pinMode(SSerialTxControl, OUTPUT);
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver   

  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate 

}

long previousMillis = 0;    // last time the sensor was checked, in ms


void loop()  
{
  long currentMillis = millis();              // current time to be compared with previousMillis
  
  if(currentMillis-previousMillis>= 2000){    // if >2 seconds from last reading. DHT22 sampling rate is 0.5Hz
    
    previousMillis=currentMillis;             // reset previousMillis

    byteSend = 0x01;                          // Send byte 0x01 to other Arduino
    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit   
    RS485Serial.write(byteReceived);          // Send byte to Remote Arduino

    delay(10);
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit       
  }

  if (RS485Serial.available())  //Look for data from other Arduino
   {
    byteReceived = RS485Serial.read();    // Read received byte
    lcd.print(byteReceived);
    delay(10);
   }

}