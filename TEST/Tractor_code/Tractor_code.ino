#include <SoftwareSerial.h>
SoftwareSerial      xbee(2,3); 
char message[2];

bool circle_status=false;
bool triangle_status=false;
bool square_status=false;
bool x_status=false;

int LeftDrive = 9 ;
int RightDrive= 8;


#include <Servo.h>

#define TX_INTERVAL 25

#define commTimeout 125 //ms

#define STEERING_MIN 620 //20
#define STEERING_MAX 1650 //120

#define steeringOffsetPin A1
#define steeringPin 4
#define drivePin 3

Servo steering,drive;
int steeringOffset;
int steeringPos;
int steeringCalc;
float sFactorMin;
float sFactorMax;
int drivePos;
int driveCalc;

byte aux, rev,brake;

unsigned long timeout;

double old_tx;

boolean datastart=false;
int bytecount=0;
byte filter[] ={0b10101010,0b01100110,0b10101010};
byte filtere[]={0b11111110,0b01111110,0b01111111};
byte inbuff[3],databuff[20];


void initializePins(){
   drive.attach(drivePin);
   steering.attach(steeringPin);
}



void setup()
{
  initializePins();
  xbee.begin(115200);
  Serial.begin(115200);
  
}

void loop(){
  getInputs();
  checkTimeout(); 
  calcSteering();
  calcDrive();
  setOutputs();
  sendData();
  }

void checkcommand(String cm , int value)
 {
   
   if(cm == "L1"){
    }
   else if (cm == "L2"){
    }
   else if (cm == "R1"){
    }
   else if (cm == "R2"){
    }
   else if (cm == "T"){
    }
   else if (cm == "S"){
    }
   else if (cm == "C"){
    }
   else if (cm == "O"){
    }
   else if (cm == "LHX"){
    }
   else if (cm == "LHY"){
    }
   else if (cm == "RHX"){
    }
   else if (cm == "RHY"){
    }
   
   
   
   

 }



void checkTimeout(){
  if(millis()>timeout+commTimeout){
    drivePos=0;
    brake=0;
    steeringPos=127;
  }
}

void sendData(){
  if(millis()>=(old_tx+TX_INTERVAL)){
    xbee.print("tick");
    old_tx=millis();
  }
}

void getInputs(){
  byte c;
  if(xbee.available())
    c=xbee.read();
   
  if(datastart){
    databuff[bytecount++]=c;
    if(bytecount>=7){
      datastart=false;
      if(databuff[3]==filtere[0]&&databuff[4]==filtere[1]&&databuff[5]==filtere[2]){
        timeout=millis();
        steeringPos=databuff[0];
        drivePos=databuff[1];
        aux=databuff[2];
        rev=aux&0b10000000;
        brake=aux&0b01000000;
      }
    }  
  } 
   
  inbuff[0]=inbuff[1];
  inbuff[1]=inbuff[2];
  inbuff[2]=c;
  if(filter[0]==inbuff[0]&&filter[1]==inbuff[1]&&filter[2]==inbuff[2]){
    datastart=true;
    bytecount=0;
  }
}

void calcSteering(){
  steeringOffset=analogRead(steeringOffsetPin)+725;
  sFactorMin=(float(STEERING_MAX-STEERING_MIN)/2.0)/(steeringOffset-STEERING_MIN);
  sFactorMax=(float(STEERING_MAX-STEERING_MIN)/2.0)/(STEERING_MAX-steeringOffset); 
  steeringCalc=(steeringOffset+(map(steeringPos,0,255,-0.5*(STEERING_MAX-STEERING_MIN),0.5*(STEERING_MAX-STEERING_MIN))/((steeringPos<=127)? sFactorMax:sFactorMin)));
  steeringCalc=min(STEERING_MAX,max(STEERING_MIN,steeringCalc));
  //Serial.print(steeringCalc);
  //Serial.print("   ");
}

void calcDrive(){
  driveCalc=1500+(map(drivePos,0,255,0,500)*((rev)?-1:1));
  if(brake)
    driveCalc=1400;
  //Serial.println(driveCalc);
}

void setOutputs(){
  //steering.write(steeringCalc);
  steering.writeMicroseconds(steeringCalc);
  drive.writeMicroseconds(driveCalc);
} 

