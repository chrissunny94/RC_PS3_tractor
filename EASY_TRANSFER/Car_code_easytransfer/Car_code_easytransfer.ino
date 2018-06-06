

#include <EasyTransfer.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

//create object
EasyTransfer ET;

int const ledpin = 13;

//data group
struct RECEIVE_DATA_STRUCTURE{

  int count;
  int left;
  int right;
  
  
};

RECEIVE_DATA_STRUCTURE txdata;

void setup(){
 
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  mySerial.begin(115200);
  ET.begin(details(txdata), &mySerial); //start library
  Serial.print("Module Started");


}

void loop(){
  if(ET.receiveData()){ //check for incoming data

    
   
    Serial.println(txdata.left);
    Serial.println(txdata.right);

    switch(txdata.count){
      
      case 0:
        if (txdata.left>0){
           digitalWrite(5,txdata.left);
           digitalWrite(6,0);
        }
        else{
           digitalWrite(6,-1*txdata.left);
           digitalWrite(5,0);}
        if (txdata.right>0){
           digitalWrite(9,txdata.right);
           digitalWrite(10,0);}
        else{
           digitalWrite(10,-1*txdata.right);
           digitalWrite(9,0);}
        break;
        
        
        default:
          Serial.println();
          
          digitalWrite(5,0);
          digitalWrite(6,0);
          digitalWrite(9,0);
          digitalWrite(10,0);
      }   
   
  }
  
}

