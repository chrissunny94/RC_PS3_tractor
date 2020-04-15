

#include <EasyTransfer.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  150  // Minimum pulse length count out of 4096.
#define SERVOMAX  700 // Maximum pulse length count out of 4096.
int servoNo = 0; // Defines a counter for servos.
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();   // Initiates library.


SoftwareSerial mySerial(2, 3); // RX, TX

//create object
EasyTransfer ET;

int const ledpin = 13;
int val;

bool Triangle = false ;
int last_press ;
bool Square = false;
bool Cross = false;
bool Circle = false;

int left_drive;
int right_drive;

int lift_actuator = 0;
int tilt_actuator = 1;

int boom_lift = 2;
int swing_actuator = 3;
int bucket_actuator = 4;
int dipper_actuator = 5;

int starter = 6;
int stopper = 7;

//data group
struct RECEIVE_DATA_STRUCTURE{

  int count;
  int left;
  int right;
  
  
};

RECEIVE_DATA_STRUCTURE txdata;



void setup(){
 
  Serial.begin(115200);
  
  mySerial.begin(115200);
  ET.begin(details(txdata), &mySerial); //start library
  Serial.print("Module Started");
  pwm.begin();         // Sends PWM signals.
  pwm.setPWMFreq(60);  // Makes servos run at 60 Hz rate.
  delay(20);


}


void loop(){
  
  
  if(ET.receiveData()){ //check for incoming data

    
    Serial.println(txdata.count);
    Serial.println(txdata.left);
    Serial.println(txdata.right);
    Serial.println(Triangle);

    switch(txdata.count){
      
      case 0:
         
         
         
         //delay(300);
        //left_drive.write(map(txdata.left, -255, 255, 0, 255)); 
        //right_drive.write(map(txdata.right, -255, 255, 0, 255)); 
        break;
      case 1:
      
        if (Triangle){
          pwm.setPWM(boom_lift, 0, map(txdata.right, 0, 255, SERVOMIN, SERVOMAX));
          pwm.setPWM(swing_actuator, 0, map(txdata.left, 0, 255, SERVOMIN, SERVOMAX));
          }

          else
          Serial.println("Disbled");
          break;
      case 2:
      
        if (Triangle){
          
          pwm.setPWM(lift_actuator, 0, map(txdata.right, 0, 255, SERVOMIN, SERVOMAX));
          pwm.setPWM(tilt_actuator, 0, map(txdata.left, 0, 255, SERVOMIN, SERVOMAX));
          
          //bucket_actuator.write(map(txdata.right, -255, 255, 0, 255)); 
          }
          else
            pwm.setPWM(tilt_actuator, 0, map(txdata.right, 0, 255, SERVOMIN, SERVOMAX));
          break;
       case 3:

            if (Triangle){
              pwm.setPWM(bucket_actuator, 0, map(txdata.right, 0, 255, SERVOMIN, SERVOMAX));
              pwm.setPWM(dipper_actuator, 0, map(txdata.left, 0, 255, SERVOMIN, SERVOMAX));
          
              }

            else
              break;
       
       case 5:

       
          if (txdata.left)
            Square = !Square;
          if (txdata.right)
             Cross = !Cross;

            if (Cross)
             // digitalWrite(HIGH , starter);
            break;
       case 6:
          if (txdata.left)
            Circle = !Circle;
             if (Circle)
              digitalWrite(HIGH , stopper);
          if (txdata.right )
            Triangle = !Triangle;
          //last_press = txdata.right;  
          break;
        default:
          Serial.println();
          break;
           
      }   
   
  }
  
}
