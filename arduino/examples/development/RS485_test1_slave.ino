/*
RS485
18/10/19
*/
#include<SoftwareSerial.h> 

#define MAX_PIN 3  //MAX485 control pin

SoftwareSerial RS485(7, 8);

String id = "001";
char type = '0';
char status_device = '1';
    
String msg;

    
void setup(){
  Serial.begin(9600);
  RS485.begin(9600);
  RS485.setTimeout (100);
  pinMode(MAX_PIN, OUTPUT);
  digitalWrite(MAX_PIN, LOW);  //Disable max485 MAX_PIN

}
    
void loop(){
  ReceiveMsg();
}      

void ReceiveMsg(){
  if(RS485.available()){         // run code only if data received from Master
    delay(1);                      // allow time for message to be received in full
    
    String masterRequest = "";     // accumulate char bytes of data from Master in String
    while (RS485.available()){          
      char c = RS485.read();
      masterRequest += c;
    }
    Serial.println(masterRequest);
    
    if (masterRequest == id){
      SendMsg();
    }
  }
}

void SendMsg(){
  int size_msg = id.length();
  char char_array[size_msg];
  id.toCharArray(char_array, size_msg);
  char msg[10] = {'[',id[0],id[1],id[2],'.',type,'.',status_device,']'};

  digitalWrite(MAX_PIN, HIGH);  //Enable max485 transmission
  //RS485.write(msg);  
  RS485.write("[001.0.1]");  
  RS485.flush();
  digitalWrite(MAX_PIN,LOW);    //Disable max485 transmission mode
}