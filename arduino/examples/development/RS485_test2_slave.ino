#include <SoftwareSerial.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        7  //Serial Receive pin
#define SSerialTX        8  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/*-----( Declare Variables )-----*/
int byteReceived;
int byteSend;

void setup()   
{
  pinMode(SSerialTxControl, OUTPUT);

  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver

  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate 
}//--(end setup )---


void loop()   
{
  //Copy input data to output  
  if (RS485Serial.available())
  {
    byteSend = RS485Serial.read();   // Read the byte 

    delay(10);

    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit    
    RS485Serial.write(byteSend); // Send the byte back
    RS485Serial.flush();
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit      
//    delay(100);
  }

}