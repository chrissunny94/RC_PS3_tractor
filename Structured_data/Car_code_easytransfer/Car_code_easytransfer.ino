

#include <EasyTransfer.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

//create object
EasyTransfer ET;

int const ledpin = 13;

//data group
struct RECEIVE_DATA_STRUCTURE{

  int l1;
  int l2;
  int r1;
  int r2;
  int lx;
  int ly;
  int rx;
  int ry;
  int x;
  int s;
  int c;
  int t;
  int up;
  int down;
  int left;
  int right;
  
  
};

RECEIVE_DATA_STRUCTURE txdata;

void setup(){
 
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  mySerial.begin(115200);
  ET.begin(details(txdata), &mySerial); //start library
  Serial.print("Module Started");


}

void loop(){
  if(ET.receiveData()){ //check for incoming data

    
   
    Serial.print(txdata.l2);
   
  }
}

