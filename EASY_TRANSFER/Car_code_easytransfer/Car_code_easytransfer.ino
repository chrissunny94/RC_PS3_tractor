

#include <EasyTransfer.h>
#include <SoftwareSerial.h>
#include <Servo.h>


SoftwareSerial mySerial(2, 3); // RX, TX

//create object
EasyTransfer ET;

int const ledpin = 13;
int val;

bool Triangle = false ;
bool Square = false;
bool Cross = false;
bool Circle = false;

Servo left_drive;
Servo right_drive;

Servo lift_actuator;
Servo tilt_actuator;

Servo boom_lift;
Servo swing_actuator;
Servo bucket_actuator;
Servo dipper_actuator;

//data group
struct RECEIVE_DATA_STRUCTURE{

  int count;
  int left;
  int right;
  
  
};

RECEIVE_DATA_STRUCTURE txdata;

void init_pins(){
  
  left_drive.attach(4);
  right_drive.attach(5);

  lift_actuator.attach(6);
  tilt_actuator.attach(7);

  boom_lift.attach(8);
  swing_actuator.attach(9);
  bucket_actuator.attach(10);
  dipper_actuator.attach(10);
  
  }

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

      
        left_drive.write(map(txdata.left, -255, 255, 0, 255)); 
        right_drive.write(map(txdata.right, -255, 255, 0, 255)); 

      case 1:
        if (Triangle){
          swing_actuator.write(map(txdata.left, -255, 255, 0, 255)); 
          boom_lift.write(map(txdata.right, -255, 255, 0, 255)); 
          }

      case 2:
        if (Triangle){
          dipper_actuator.write(map(txdata.left, -255, 255, 0, 255)); 
          bucket_actuator.write(map(txdata.right, -255, 255, 0, 255)); 
          }
       
       case 5:
          if (txdata.left)
            Square = !Square;
          if (txdata.right)
            Cross = !Cross;

       case 6:
          if (txdata.left)
            Circle = !Circle;
          if (txdata.right)
            Triangle = !Triangle;
             
        
        default:
          Serial.println();
          
          digitalWrite(5,0);
          digitalWrite(6,0);
          digitalWrite(9,0);
          digitalWrite(10,0);
      }   
   
  }
  
}

