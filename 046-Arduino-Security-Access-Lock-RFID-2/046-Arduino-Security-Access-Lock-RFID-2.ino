byte mystr[4];
byte db[4][4];
int count = 0, a, i;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
    Serial.readBytes(mystr,4);
    Serial.print("Scanned Code");
    for(int i=0;i<4;i++){
      Serial.print(mystr[i], HEX);
      Serial.print(" ");
      delay(1000);
   }
   Serial.println("");

  for(i=0;i<4;i++){
      if(db[i][0]==mystr[0]){                     //same tag scanned again
        break;
      }
      else if(db[i][0]==NULL){
        for(j=0;j<4;i++){
          db[i][j]=mystr[j];
        }
        break;
      }
  }      
   
  /* if(db[0][0]==NULL){     // empty db
   for(i=0;i<4;i++){
    db[0][i]=mystr[i];}
    count++;
    }
   else{                            // not empty
    for(i=0;i<4;i++){
      if(db[i][0]==mystr[0]){                     //same tag scanned again
        a = 0;
        break;
        }
       else{                                   
        for(int i=0; i<4; i++)
      db[count][i]=mystr[i];
      
    }}} */
    for(i =0; i<4; i++){
    for(int j=0; j<4;j++){
      Serial.print(db[i][j], HEX);
      Serial.print(" ");
      delay(1000);
      }
      Serial.println("");
    }
   //Serial.println(db[i], HEX);}}
  // put your main code here, to run repeatedly:

}
