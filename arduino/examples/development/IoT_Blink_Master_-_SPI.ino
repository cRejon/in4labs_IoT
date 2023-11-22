#include <SPI.h>

#define SS 10                   
#define BLINK_DELAY 2000          // time in ms that the led is on/off


SPISettings mySettings(1000000, MSBFIRST, SPI_MODE0); // 1 MHz clock, MSB first, mode 0

void setup() {
  digitalWrite(SS, HIGH);       // https://code.google.com/archive/p/arduino/issues/888
  pinMode(SS,OUTPUT);            
  

  SPI.begin();                    // SPI begin
}

void loop() {

  SPI.beginTransaction(mySettings);
  digitalWrite(SS, LOW);  
  delay(100);
  SPI.transfer(0x01);   
  delay(100); 
  digitalWrite(SS, HIGH); 
  SPI.endTransaction();

  delay(BLINK_DELAY);            
    
  SPI.beginTransaction(mySettings);
  digitalWrite(SS, LOW);   
  delay(100);  
  SPI.transfer(0x00); 
  delay(100);
  digitalWrite(SS, HIGH); 
  SPI.endTransaction();

  delay(BLINK_DELAY);             
}