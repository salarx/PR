#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define BUZZER 2 //buzzer pin

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(3); //servo pin
  myServo.write(0); //servo start position
  pinMode(LED_G, OUTPUT);//setup led 
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  //Serial.println("Put your card to the reader...");
  //Serial.println();
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {    
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  //Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    // Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println();
  //Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "63 0B D5 83" || content.substring(1) == "76 F5 D3 83" || content.substring(1) == "74 0C 5C D3" || content.substring(1) == "CA 9A 5D D3") //change here the UID of the card/cards that you want to give access
  {

    if(mfrc522.uid.uidByte[0]==0x63
    &&mfrc522.uid.uidByte[1]==0x0B
    &&mfrc522.uid.uidByte[2]==0xD5
    &&mfrc522.uid.uidByte[3]==0x83) {
  
    for(int i = 0; i < 4; i++) {
      Serial.write(mfrc522.uid.uidByte[i]);
      }
    }
  
    if(mfrc522.uid.uidByte[0]==0x76
    &&mfrc522.uid.uidByte[1]==0xF5
    &&mfrc522.uid.uidByte[2]==0xD3
    &&mfrc522.uid.uidByte[3]==0x83) {
  
      for(int i = 0; i < 4; i++) {
        Serial.write(mfrc522.uid.uidByte[i]);
      }
    }
    
    if(mfrc522.uid.uidByte[0]==0xCA
    &&mfrc522.uid.uidByte[1]==0x9A
    &&mfrc522.uid.uidByte[2]==0x5D
    &&mfrc522.uid.uidByte[3]==0xD3) {
  
      for(int i = 0; i < 4; i++) {
        Serial.write(mfrc522.uid.uidByte[i]);
      }
    }
  
    if(mfrc522.uid.uidByte[0]==0x74
    &&mfrc522.uid.uidByte[1]==0x0C
    &&mfrc522.uid.uidByte[2]==0x5C
    &&mfrc522.uid.uidByte[3]==0xD3) {
  
      for(int i = 0; i < 4; i++) {
        Serial.write(mfrc522.uid.uidByte[i]);
      }    
    }
  
    //Serial.println("Authorized access");
    //Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(90);
    delay(4000);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
  }
 
  else {
    //Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(800);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}
