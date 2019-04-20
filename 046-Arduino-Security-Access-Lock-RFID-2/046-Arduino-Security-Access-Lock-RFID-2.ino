byte mystr[4];
byte db[4][4];
int j, i, count=0, a;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

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
      if(db[i][0]==mystr[0]){                     //same tag scanned again
        break;
      }
      else if(db[i][0]==0){
        for(j=0;j<4;j++){
          db[i][j]=mystr[j];
        }
        break;
      }
  }      
   
/*   if(db[0][0]==NULL){     // empty db
   for(i=0;i<4;i++){
    db[0][i]=mystr[i];}
    count++;
    }
   else{                            // not empty
    for(i=0;i<4;i++){
      if(db[i][0]==mystr[0]){                     
        a = 5;
        break;
        }
    }
     for(int p=0;p<4;p++){
       if(db[p][0]!=mystr[0])
       {                                                        
        for(int j=0; j<4; j++){
      db[count-1][j]=mystr[j];}
      count++;
      
    }
    break;
     }}*/
    
    for(i =0; i<4; i++){  //print
    for(int j=0; j<4;j++){
      Serial.print(db[i][j], HEX);
      Serial.print(" ");
      delay(500);
      }
      Serial.println("");
    }
   //Serial.println(db[i], HEX);}}
  // put your main code here, to run repeatedly:

}
