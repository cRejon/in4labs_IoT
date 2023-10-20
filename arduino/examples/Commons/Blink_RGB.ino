/*
  Sketch name: Blink RGB
  UNED Arduino IoT lab.

  Cicles a RGB LED through a set of different colors.

  Function 'analogWrite' is used to control LED intensity and mix colors by PWM:
    - MIN intensity is 0 (OFF)
    - MAX intensity is 256 (ON) 
  
  Circuit:
    - RGB LED connected to pins 5 (red), 6 (green) and 9 (blue)
*/

// definitions
#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

#define DELAY 1000       // time in ms that the LEd is set in each color

// initialize led pins as an output
void setup() {
  pinMode(RED_PIN, OUTPUT);   // red
  pinMode(GREEN_PIN, OUTPUT); // green
  pinMode(BLUE_PIN, OUTPUT);  // blue
}

// the loop function runs over and over again forever
void loop() {
  setColor("YELLOW"); 
  delay(DELAY);  
  
  setColor("ORANGE"); 
  delay(DELAY);
  
  setColor("RED");        
  delay(DELAY);      
 
  setColor("MAGENTA");        
  delay(DELAY);    

  setColor("BLUE");        
  delay(DELAY); 
     
  setColor("CYAN");        
  delay(DELAY);    

  setColor("GREEN");        
  delay(DELAY);   
}

void setColor(String color) {
  if( color == "RED"){
      analogWrite(RED_PIN, 256);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
  } else if( color == "MAGENTA"){
      analogWrite(RED_PIN,128);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 128);
  } else if( color == "BLUE"){
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 256);
  } else if( color == "CYAN"){
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 128);
      analogWrite(BLUE_PIN, 128);
  } else if( color == "GREEN"){
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 256);
      analogWrite(BLUE_PIN, 0);
  } else if( color == "YELLOW"){
      analogWrite(RED_PIN, 128);
      analogWrite(GREEN_PIN, 128);
      analogWrite(BLUE_PIN, 0);
  } else if( color == "ORANGE"){
      analogWrite(RED_PIN, 256);
      analogWrite(GREEN_PIN, 74);
      analogWrite(BLUE_PIN, 0);
  } else { //OFF
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
  }
}
