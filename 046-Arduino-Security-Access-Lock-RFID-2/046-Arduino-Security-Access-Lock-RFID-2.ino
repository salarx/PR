byte mystr[4];
byte db[4][4];
int j, i, count=0, a;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
    Serial.readBytes(mystr,4);                    //print the scanned TAG
    Serial.print("Scanned Code ");
    for(int i=0;i<4;i++){
      Serial.print(mystr[i], HEX);
      Serial.print(" ");
      delay(500);
   }
   Serial.println("");

  for(i=0;i<4;i++){                                //fill tag in the database 
      if(db[i][0]==mystr[0]){                     
        break;
      }
      else if(db[i][0]==0){
        for(j=0;j<4;j++){
          db[i][j]=mystr[j];
        }
        break;
      }
  }      

    
    for(i =0; i<4; i++){                              //print database
    for(int j=0; j<4;j++){
      Serial.print(db[i][j], HEX);
      Serial.print(" ");
      delay(500);
      }
      Serial.println("");
    }
  

}
