#include <SPI.h>
#include <MFRC522.h>
#include <Chrono.h>

// PIN Numbers : RESET + SDAs
#define RST_PIN         9
#define SS_1_PIN        10
#define SS_2_PIN        8
#define SS_3_PIN        7
#define SS_4_PIN        6

// Led and Relay PINS
#define LED_1 2
#define LED_2 3
#define LED_3 4
#define LED_4 5

// List of Tags UIDs
byte scannedarray[4][4];
byte tagarray[4][4];

byte mystr[4];
int j, i;

Chrono timefin[4];
bool present = false;
bool rescanned = false;

#define NR_OF_READERS   4

byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN};

// Create an MFRC522 instance :
MFRC522 mfrc522[NR_OF_READERS];

/**
   Initialize.
*/
void setup() {

  Serial.begin(9600);           // Initialize serial communications with the PC
  while (!Serial);              // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();                  // Init SPI bus

  /* looking for MFRC522 readers */
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
}

/*
   Main loop.
*/
void loop() {
  for(i = 0; i < 4; i++) {
    if(timefin[i].hasPassed(20000) && tagarray[i][0]!=0) {
      Serial.print(timefin[i].elapsed());
      Serial.print(" : ");
      for(j = 0; j < 4; j++) {
        Serial.print(tagarray[i][j], HEX);
        Serial.print(" ");
      }
      Serial.println(":is not parked in parking zone");
    }
  }

  Serial.readBytes(mystr,4);
  Serial.print("Scanned Code ");
  for(int i = 0; i < 4; i++) {
    Serial.print(mystr[i], HEX);
    Serial.print(" ");
    delay(300);
  }
  Serial.println("");
    
  for(i = 0; i < 4; i++) {
    if(mystr[0] == 0){
      break;
    }
    if(tagarray[i][0] == mystr[0]) {  //same tag scanned again 
      rem_byte_tagarray(i);
      rem_byte_mystr();
      break;
    }
    if(tagarray[i][0] == 0) {
      for(j = 0; j < 4; j++) {
        tagarray[i][j] = mystr[j];
      }
      rem_byte_mystr();
      timefin[i].restart();
      break;
    }
  }  //end of database
    
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {

    // Looking for new cards
    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      Serial.print(reader);

      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(reader, mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
    }
    else if (!mfrc522[reader].PICC_IsNewCardPresent()) {
      rem_byte_array(reader);
    }
  }

  for (j = 0; j < 4; j++) { //no parking
    present = false; 
    for (i = 0; i < 4; i++) {
      if(tagarray[j][0]==scannedarray[i][0] && scannedarray[i][0]!=0 && tagarray[j][1]==scannedarray[i][1]) {
        present = true;
        timefin[j].restart();
        Serial.println("Successfully parked.");
        break;
      }
    }
      
    if(present == false) {
      for(int h = 0; h < 4; h++) {
        Serial.print(tagarray[j][h], HEX);
        Serial.print(" ");
        delay(300);
      }
      Serial.println(" is un-parked");
      Serial.println("");
    }     
  }

  for (i = 0; i < 4; i++) {
    if(scannedarray[i][0] != 0) {
      if(i == 0) {
        digitalWrite(LED_1, HIGH);
      }
      if(i == 1) {
        digitalWrite(LED_2, HIGH);
      }
      if(i == 2) {
        digitalWrite(LED_3, HIGH);
      }
      if(i == 3) {
        digitalWrite(LED_4, HIGH);
      }
    }

    else {
      if(i == 0) {
        digitalWrite(LED_1, LOW);
      }
      if(i == 1) {
        digitalWrite(LED_2, LOW);
      }
      if(i == 2) {
        digitalWrite(LED_3, LOW);
      }
      if(i == 3) {
        digitalWrite(LED_4, LOW);
      }
    }    
  }  
}

/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void dump_byte_array(uint8_t reader,byte * buffer, byte bufferSize) {
  for(j = 0; j < 4; j++) {
    scannedarray[reader][j] = buffer[j];
  }
  
  for(byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }

  Serial.println(" Scanned Array : ");
  Serial.println("");
  for(i = 0; i < 4; i++) {  //print
    for(int j = 0; j < 4; j++) {
      Serial.print(scannedarray[i][j], HEX);
      Serial.print(" ");
      delay(300);
    }
    Serial.println("");
  }
}

void rem_byte_array(uint8_t reader) {
  for(j = 0; j < 4; j++){
    scannedarray[reader][j] = 0;
  }  
}

void rem_byte_tagarray(int reader) {
  for(j = 0; j < 4; j++){
    tagarray[reader][j] = 0;
  }  
}

void rem_byte_mystr() {
  for(j = 0; j < 4; j++){
    mystr[j] = 0;
  }  
}
