
/*
Version using BT module 
BT is connected to serial  
stepper is connected STEP = 5 DIR = 6


By Alban P.

 */

#define stp 5
#define dir 6
  String readString;
  String code = "1865";
  String Read;
  boolean Open = true;

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

void setup() {

  // set the speed at 60 rpm:
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
}

void loop() {
   while (Serial.available()) {
     delay(10);  
     if (Serial.available() >0) {
       char c = Serial.read();  //gets one byte from serial buffer
       readString += c; //makes the string readString
     }
   }

  if (readString.length() >0) {
     // expect a string like 07002100 containing the two servo positions      
     Read = readString.substring(0, 4); //get the first four character     
     Serial.println(Read);  //print ot serial monitor to see results

     if (Read==code){
      Action();
     }
     
     readString="";
  }
  

}

void Action() {
  if(Open) {
    closeDoor();
  } else {
    openDoor();
  }
}

void openDoor() {
  Serial.println("Ouverture");
  digitalWrite(dir,HIGH);
  for(int x=0;x<stepsPerRevolution;x++) {
    digitalWrite(stp,HIGH); 
    delay(5); 
    digitalWrite(stp,LOW); 
    delay(5); 
  }
  Open = true;
  delay(500);
}

void closeDoor(){
  Serial.println("Fermeture");
  digitalWrite(dir,LOW); //Making 1 revolution in 2s. 
  for(int x=0;x<stepsPerRevolution;x++) {
    digitalWrite(stp,HIGH); 
    delay(5); 
    digitalWrite(stp,LOW); 
    delay(5); 
  }
  Open = false;
  delay(500);
}

