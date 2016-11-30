

/*
NFC Door
al26
https://github.com/al26p 

Based on https://www.hackster.io/keebie81/rfid-door-unlock-fa66ea & http://www.instructables.com/id/Arduino-RFID-Door-Lock/?ALLSTEPS

*/

#include <EEPROM.h> //lib for read/write in EEPROM memory
#include <SPI.h>
#include <MFRC522.h>


#define failPin 11 // Red LED
#define passPin 12 // Grenn LED
 

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

//Initialisation
boolean programMode = false; // Initialize program mode to false
boolean deleteMode = false; // Initialize delete mode to false
boolean wipeMode = false; // Initialize wipe mode to false
boolean match = false; // initialize card match to false
boolean doorClosed = true;

byte storedCard[6]; // Stores an ID read from EEPROM
byte readCard[6]; // Sotres an ID read from the RFID reader
byte checksum = 0; // Stores the checksum to verify the ID 


void setup() {
  pinMode(passPin, OUTPUT); // Connected to Green on tri-color LED to indicate user is valid
  pinMode(failPin, OUTPUT); // Connected to Red on tri-color LED to indicate user is NOT valid or read failed
  pinMode(doorPin, OUTPUT); // Connected to relay to activate the door lock
  Init();
  Serial.begin(9600); // Connect to the serial port
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

   
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  finInit();
}

void loop () {
  byte val = 0; // Temp variable to hold the current byte
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.println();
  Serial.println(F("A new card has been detected."));
  for (byte i = 0; i < 5; i++) {
    readCard[i] = rfid.uid.uidByte[i];
    }

  Serial.println(F("The NUID tag is:"));
  Serial.print(F("In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  
}

void Init() {
  digitalWrite(passPin, HIGH); // Make sure Green LED is on
  digitalWrite(failPin, HIGH); // Make sure Red LED is on
}

void finInit() {
  digitalWrite(passPin, LOW); // Make sure Green LED is off
  digitalWrite(failPin, LOW); // Make sure Red LED is off 
}
