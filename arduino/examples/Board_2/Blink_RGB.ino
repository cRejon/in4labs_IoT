#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

const int redPin = 26;
const int greenPin = 25;
const int bluePin = 27;

void setup() {
  WiFiDrv::pinMode(25, OUTPUT); 
  WiFiDrv::pinMode(26, OUTPUT); 
  WiFiDrv::pinMode(27, OUTPUT); 
}

void turn_off_led(){
  WiFiDrv::analogWrite(greenPin, 0);
  WiFiDrv::analogWrite(redPin, 0);
  WiFiDrv::analogWrite(bluePin, 0);
}

void loop() {
  turn_off_led();
  delay(1000);
  WiFiDrv::analogWrite(greenPin, 255);
  WiFiDrv::analogWrite(redPin, 0);
  WiFiDrv::analogWrite(bluePin, 0);
  delay(1000);
  turn_off_led();
  delay(1000);
  WiFiDrv::analogWrite(greenPin, 0);
  WiFiDrv::analogWrite(redPin, 255);
  WiFiDrv::analogWrite(bluePin, 0);
  delay(1000);
  turn_off_led();
  delay(1000);
  WiFiDrv::analogWrite(greenPin, 0);
  WiFiDrv::analogWrite(redPin, 0);
  WiFiDrv::analogWrite(bluePin, 255);
  delay(1000);
}