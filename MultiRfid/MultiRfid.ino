
/**
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program showing how to read data from more than one PICC to serial.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   Example sketch/program showing how to read data from more than one PICC (that is: a RFID Tag or Card) using a
   MFRC522 based RFID Reader on the Arduino SPI interface.

   Warning: This may not work! Multiple devices at one SPI are difficult and cause many trouble!! Engineering skill
            and knowledge are required!

   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required *
   SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required *
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

*/

#include <SPI.h>
#include <MFRC522.h>

// PIN Numbers : RESET + SDAs
#define RST_PIN         9
#define SS_1_PIN        10
#define SS_2_PIN        8
#define SS_3_PIN        7
#define SS_4_PIN        6

// Led and Relay PINS
#define GreenLed        2
#define relayIN         3
#define RedLed          4


// List of Tags UIDs that are allowed to open the puzzle
byte scannedarray[4][4];
byte tagarray[4][4];
byte mystr[4];
int j, i;
bool present = false;

// Inlocking status :
int tagcount = 0;
bool access = false;

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
    //mfrc522[reader].PCD_SetAntennaGain(mfrc522[reader].RxGain_max);
  }
}

/*
   Main loop.
*/

void loop() {

  Serial.readBytes(mystr,4);
    Serial.print("Scanned Code ");
    for(int i=0;i<4;i++){
      Serial.print(mystr[i], HEX);
      Serial.print(" ");
      delay(500);
    }
    Serial.println("");

    for(i=0;i<4;i++){
      if(tagarray[i][0]==mystr[0]){                     //same tag scanned again
        break;
      }
      else if(tagarray[i][0]==0){
        for(j=0;j<4;j++){
          tagarray[i][j]=mystr[j];
        }
        break;
      }
    }  //end of db array

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {

    // Looking for new cards
    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      Serial.print(reader);

      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(reader, mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();

     /* for (int x = 0; x < sizeof(tagarray); x++)                  // tagarray's row
      {
        for (int i = 0; i < mfrc522[reader].uid.size; i++)        //tagarray's columns
        {
          if ( mfrc522[reader].uid.uidByte[i] != tagarray[x][i])  //Comparing the UID in the buffer to the UID in the tag array.
          {
            DenyingTag();
            break;
          }
          else
          {
            if (i == mfrc522[reader].uid.size - 1)                // Test if we browesed the whole UID.
            {
              AllowTag();
            }
            else
            {
              continue;                                           // We still didn't reach the last cell/column : continue testing!
            }
          }
        }
        if (access) break;                                        // If the Tag is allowed, quit the test.
      }


      if (access)
      {
        if (tagcount == NR_OF_READERS)
        {
          OpenDoor();
        }
        else
        {
          MoreTagsNeeded();
        }
      }
      else
      {
        UnknownTag();
      } */
     // Serial.print(F("PICC type: "));
       // MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
       // Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));
      // Halt PICC   
     // mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
     // mfrc522[reader].PCD_StopCrypto1();
    }
    else{
      rem_byte_array(reader);
      }
      
    //if (mfrc522[reader].PICC_IsNewC..
  } //for(uint8_t reader..
  for (j = 0; j < 4; j++) {
    present = false; 
    for (i = 0; i < 4; i++){
      if(tagarray[j][0]==scannedarray[i][0] && tagarray[j][1]==scannedarray[i][1]){
        present = true;
        break;
      }
      }
      if(present == false){
            //print
    for(int h=0; h<4;h++){
      Serial.print(tagarray[j][h], HEX);
      Serial.print(" ");
      delay(500);
      }
      Serial.println(" is non parked");
      Serial.println("");
      }     
}
}

/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void rem_byte_array(uint8_t reader) {
  for(j=0;j<4;j++){
          scannedarray[reader][j]=0;
        }  
  }

void dump_byte_array(uint8_t reader,byte * buffer, byte bufferSize) {
        for(j=0;j<4;j++){
          scannedarray[reader][j]=buffer[j];
        }
  
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println("");
  Serial.println("Scanned Array : ");
  for(i = 0; i<4; i++){  //print
    for(int j=0; j<4;j++){
      Serial.print(scannedarray[i][j], HEX);
      Serial.print(" ");
      }
      Serial.println("");
    }
}

/*void printTagcount() {
  Serial.print("Tag n°");
  Serial.println(tagcount);
}

void DenyingTag()
{
  tagcount = tagcount;
  access = false;
}

void AllowTag()
{
  tagcount = tagcount + 1;
  access = true;
}

void Initialize()
{
  tagcount = 0;
  access = false;
}

void OpenDoor()
{
  Serial.println("Welcome! the door is now open");
  Initialize();
  digitalWrite(relayIN, LOW);
  digitalWrite(GreenLed, HIGH);
  delay(2000);
  digitalWrite(relayIN, HIGH);
  delay(500);
  digitalWrite(GreenLed, LOW);
}

void MoreTagsNeeded()
{
  printTagcount();
  Serial.println("System needs more cards");
  digitalWrite(RedLed, HIGH);
  delay(1000);
  digitalWrite(RedLed, LOW);
  access = false;
}

void UnknownTag()
{
  Serial.println("This Tag isn't allowed!");
  printTagcount();
  for (int flash = 0; flash < 5; flash++)
  {
    digitalWrite(RedLed, HIGH);
    delay(100);
    digitalWrite(RedLed, LOW);
    delay(100);
  }
}
*/
