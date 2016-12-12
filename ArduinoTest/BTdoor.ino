
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#define stp 5
#define dir 6
#define button 3
  String readString;
  String code = "1865";
  String Read;
  boolean Open = true;
  int buttonState = 0;

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

void setup() {

  // set the speed at 60 rpm:
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  buttonState = digitalRead(button);
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

   if (buttonState == HIGH) {
    Action();
    delay(1000);
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

