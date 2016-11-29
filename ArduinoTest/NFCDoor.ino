

/*
NFC Door
al26
https://github.com/al26p 

Based on https://www.hackster.io/keebie81/rfid-door-unlock-fa66ea & http://www.instructables.com/id/Arduino-RFID-Door-Lock/?ALLSTEPS

*/

#include <EEPROM.h> //lib for read/write in EEPROM memory

#define failPin 11 // Red LED
#define passPin 12 // Grenn LED
#define doorPin 9 // Relay 
// Don't know if used #define alarmPin 13 // Alarm

//Initialisation
boolean programMode = false; // Initialize program mode to false
boolean deleteMode = false; // Initialize delete mode to false
boolean wipeMode = false; // Initialize wipe mode to false
boolean match = false; // initialize card match to false

byte storedCard[6]; // Stores an ID read from EEPROM
byte readCard[6]; // Sotres an ID read from the RFID reader
byte checksum = 0; // Stores the checksum to verify the ID 


void setup() {
  pinMode(passPin, OUTPUT); // Connected to Green on tri-color LED to indicate user is valid
  pinMode(failPin, OUTPUT); // Connected to Red on tri-color LED to indicate user is NOT valid or read failed
  pinMode(doorPin, OUTPUT); // Connected to relay to activate the door lock
//  pinMode(alarmPin, OUTPUT); // Connected to alarm
  alarm = 0;
  Serial.begin(9600); // Connect to the serial port
}

void loop () {
  byte val = 0; // Temp variable to hold the current byte
  normalModeOn();
  
}

void normalModeOn() {
  digitalWrite(passPin, LOW); // Make sure Green LED is off
  digitalWrite(failPin, LOW); // Make sure Red LED is off
  digitalWrite(doorPin, LOW); // Make sure Door is Locked 
}
