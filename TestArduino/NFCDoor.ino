/*
NFC Door
al26
https://github.com/al26p 

Based on https://www.hackster.io/keebie81/rfid-door-unlock-fa66ea & http://www.instructables.com/id/Arduino-RFID-Door-Lock/?ALLSTEPS

*/

#include // Needed to write to EEPROM storage

#define powerPin 10 // Blue LED
#define failPin 11 // Red LED
#define passPin 12 // Grenn LED
#define doorPin 9 // Relay 
#define alarmPin 13 // Alarm

boolean programMode = false; // Initialize program mode to false
boolean deleteMode = false; // Initialize delete mode to false
boolean wipeMode = false; // Initialize wipe mode to false
boolean match = false; // initialize card match to false

byte storedCard[6]; // Stores an ID read from EEPROM
byte readCard[6]; // Sotres an ID read from the RFID reader
byte checksum = 0; // Stores the checksum to verify the ID 

int alarm = 0; // Extra Security 

void setup() {
  pinMode(powerPin, OUTPUT); // Connected to Blue on tri-color LED to indicate reader is ready
  pinMode(passPin, OUTPUT); // Connected to Green on tri-color LED to indicate user is valid
  pinMode(failPin, OUTPUT); // Connected to Red on tri-color LED to indicate user is NOT valid or read failed
  pinMode(doorPin, OUTPUT); // Connected to relay to activate the door lock
  pinMode(alarmPin, OUTPUT); // Connected to alarm
  alarm = 0;
  Serial.begin(9600); // Connect to the serial port
}

void loop () {
  byte val = 0; // Temp variable to hold the current byte
  normalModeOn(); // Normal mode, blue Power LED is on, all others are off
  if (alarm == 3) {
    digitalWrite(alarmPin, HIGH); // Alarm!
    Serial.begin(9600);
    if(Serial.available() > 0) {    // Waits for something to come on the serial line
      if((val = Serial.read()) == 2) {    // First Byte should be 2, STX byte 
        getID(); // Get the ID, sets readCard = to the read ID
        if ( isMaster(readCard) )  {  // Check to see if it is the master programing card or delete card
          digitalWrite(alarmPin, LOW);
          alarm = 0;
          checksum = 0;
        }
        else
        checksum = 0;
      }
    }
    checksum=0;
  }
  else {
    if ( programMode) {  // Program mode to add a new ID card
      programModeOn(); // Program Mode cycles through RGB waiting to read a new card
      if(Serial.available() > 0) { // Waits for something to come on the serial line
        if((val = Serial.read()) == 2) { // First Byte should be 2, STX byte 
          getID(); // Get the ID, sets readCard = to the read ID
          if ( isMaster(readCard) || isDelete(readCard) || isWipe(readCard)) {  // Check to see if it is the master programing card or delete card
            programMode = false;
            if (isMaster(readCard))
            openDoor(2);
            else
            failedWrite();
            checksum = 0;
          }
          else {
            writeID(readCard); // If not, write the card to the EEPROM sotrage
            programMode = false; // Turn off programing mode
            checksum = 0; // Make sure the checksum is empty
          }
        }
      }
    }
  else if ( deleteMode ) { // Delete mode to delete an added ID card
    deleteModeOn(); // Delete Mode cycles through RB waiting to read a new card
    if(Serial.available() > 0)  { // Waits for something to come on the serial line
      if((val = Serial.read()) == 2) { // First Byte should be 2, STX byte 
        getID(); // Get the ID, sets readCard = to the read ID
        if ( isMaster(readCard) || isDelete(readCard) || isWipe(readCard) ) { // Check to see if it is the master programing card or the Delete Card
          deleteMode = false;
          checksum = 0;
          failedWrite();
        }
      else { //if ( !isMaster(readCard) && !isDelete(readCard) )
        deleteID(readCard); // If not, delete the card from the EEPROM sotrage
        deleteMode = false; // Turn off delete mode
        checksum = 0; // Make sure the checksum is empty
      }
    }
  }
}
else if ( wipeMode ) // Wipe mode to wipe out the EEPROM
{
Serial.end();
wipeModeOn();
for (int i = 0; i < 512; i++) // Loop repeats equal to the number of array in EEPROM
{
EEPROM.write(i, 0);
}
wipeMode = false;
wipeModeOn();
Serial.begin(9600);
}
// Normal Operation...
else
{
if(Serial.available() > 0) // If the serial port is available and sending data...
{
if((val = Serial.read()) == 2) // First Byte should be 2, STX byte 
{ 
getID(); // Get the ID, sets readCard = to the read ID
byte bytesread = 0;
for ( int i = 0; i < 5; i++ ) // Loop 5 times
{
if ( readCard[i] < 16 ) // Print out 0 if < 16 to prepend output
Serial.print("0");
Serial.print(readCard[i], HEX); // Print out the hex value read in
Serial.print(" ");
}
Serial.println();
Serial.print("Checksum: ");
Serial.print(readCard[5], HEX); // Checksum read from the card
if ( readCard[5] == checksum ) // See if the 5th BYTE (the checksum) read in from the reader
{ // matches the checksum caculated 
checksum = 0; // If so, we can empty the variable storing the calculated checksum
//Serial.println(" passed"); 
//Serial.println();
if ( isMaster( readCard ) ) // Check to see if the card is the master programing card
{
programMode = true; // If so, enable programing mode
alarm = 0;
}
else if ( isDelete( readCard ) ) // Check to see if the card is the deletion card
{
deleteMode = true; // If so, enable deletion mode
alarm = 0;
}
else if ( isWipe( readCard ) ) // Check to see if the card is the deletion card
{
wipeMode = true; // If so, enable deletion mode
alarm = 0;
}
else 
{
if ( findID(readCard) ) // If not, see if the card is in the EEPROM
{
openDoor(2); // If it is, open the door lock
alarm = 0;
}
else
{
failed(); // If not, show that the ID was not valid
alarm++;
}
}
}
else // If the checksum failed
{ // Print out the checksum
/*
Serial.println(" error");
Serial.println();
Serial.print("[");
Serial.print(readCard[5], HEX);
Serial.print("] != [");
Serial.print(checksum, HEX);
Serial.print("] ");
*/
}
}
}
}
}
}

// If the serial port is ready and we received the STX BYTE (2) then this function is called 
// to get the 4 BYTE ID + 1 BYTE checksum. The ID+checksum is stored in readCard[6]
// Bytes 0-4 are the 5 ID bytes, byte 5 is the checksum

