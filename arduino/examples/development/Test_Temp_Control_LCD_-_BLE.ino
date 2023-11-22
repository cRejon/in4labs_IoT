#include <ArduinoBLE.h>         // include BLE library
#include <LiquidCrystal.h>      // include LCD library

#define TEMP_LIMIT 50.0    // if this threshold is reached - instruct fan on 
#define ON 0x01            // value to write to BLE fan device to turn it on
#define OFF 0x00           // value to write to BLE fan device to turn it off

byte fanStatus = OFF;
byte previousFanStatus = OFF; 
char tempRead[5]; 
char humRead[5];

BLEDevice peripheral;                       // variable referencing a BLE peripheral device
BLECharacteristic tempCharacteristic; // variable referencing a characteristic from a BLE peripheral device
BLECharacteristic humCharacteristic; // variable referencing a characteristic from a BLE peripheral device

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  BLE.begin();                  // initialization of BLE communication

  Serial.begin(9600);           // initialization of Serial communication
  lcd.begin(16, 2);             // set up the LCD's number of columns and rows
  lcd.clear();
}

void loop() {
  
  do {
    BLE.scanForName("Arduino DHT22 Sensor");   // target BLE Sensor Service
    peripheral = BLE.available();
    delay(10);
  } while (!peripheral);

  Serial.print("Found ");
  Serial.print(peripheral.address());
  Serial.print(" '");
  Serial.print(peripheral.localName());
  Serial.print("' ");
  Serial.print(peripheral.advertisedServiceUuid());
  Serial.println();
  
  BLE.stopScan();                        // stop scanning

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  tempCharacteristic = peripheral.characteristic("2A6E"); 
  humCharacteristic = peripheral.characteristic("2A6F");

  while (peripheral.connected()) {  
 
    int temp = 0;
    tempCharacteristic.readValue(temp);
    float tempFloat = temp / 10.0;    // convert to float and divide by 10
    dtostrf(tempFloat, 4, 1, tempRead);  // convert to string with 1 decimal place
    Serial.println(tempRead);

    int hum = 0;
    humCharacteristic.readValue(hum);
    float humFloat = hum / 10.0;
    dtostrf(humFloat, 4, 1, humRead);  // convert to string with 1 decimal place
    Serial.println(humRead);

    if (tempFloat > TEMP_LIMIT ){ // if the temperature threshold is reached
        fanStatus = ON;           // update fan status to be printed by LCD screen
    } else {
        fanStatus = OFF;          // update fan status to be printed by LCD screen
    }
    
    updateLCD();                    // update LCD screen with sensor readings and fan status

    if (fanStatus != previousFanStatus){ // if fan status has changed
      peripheral.disconnect();            // disconnect from peripheral  
      instructFan(fanStatus);        // instruct fan to turn on or off
      previousFanStatus = fanStatus;      // update previous fan status
    }      
  }
}


void instructFan (byte status){  // function to write input "value" into BLE fan device
                                // switchCharacteristic. 0x00 will turn fan off
                                // any other value will turn it on. 
                                // see example "Temp Control Fan - BLE"
  do {
    BLE.scanForName("Arduino Fan");   // target BLE Fan Service
    peripheral = BLE.available();
    delay(10);
  } while (!peripheral);

  BLE.stopScan();                       // stop scanning

  if (peripheral.connect()) {  // connecting to peripheral
    delay(10);   // small delay to smooth communication
    
    if (peripheral.discoverService("19B10000-E8F2-537E-4F6C-D104768A1214")) {  
      // retrieve the FAN switchCharacteristic
      BLECharacteristic fanCharacteristic = peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1214");
      fanCharacteristic.writeValue(status);  // write value to fanCharacteristic
    }
      
    peripheral.disconnect();            // disconnect from peripheral       
  }
}

// function that updates sensor readings and fan status in LCD screen
void updateLCD() {
  // clear LCD screen
  lcd.clear();  
  // sensor readings
  lcd.setCursor(1, 0);
  lcd.print("T:");
  lcd.setCursor(3, 0);
  lcd.print(tempRead);
  lcd.setCursor(7, 0);
  lcd.print(" C");
  
  lcd.setCursor(1, 1);
  lcd.print("H:");
  lcd.setCursor(3, 1);
  lcd.print(humRead);
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
  if (fanStatus == ON) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }
}
