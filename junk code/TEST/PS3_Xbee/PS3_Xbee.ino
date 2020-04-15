
#include <PS3BT.h>
#include <usbhub.h>
#include <SoftwareSerial.h>

SoftwareSerial xbee(10, 9); // RX, TX

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

//SPI for USB host 13,12,11

#define TX_INTERVAL 5
#define RX_TIMEOUT 100
#define STATUS_BLINKRATE 250

#define deadzone_xbox 15
#define offset_xbox 7
#define deadzone_ps3 7
#define offset_ps3 2

byte filter[] ={0b10101010,0b01100110,0b10101010};
byte filtere[]={0b11111110,0b01111110,0b01111111};
byte vals[]={127,0,0b01000000};
int pos, negOff, posOff, l2, r2 , l1 , r1 ,lhx ,rhx ,lhy , rhy;
int speed_state=0;

double old_tx;
double old_rx;
double old_blink;

double avg=4;
boolean lowbat=false;

int i=0;





USB Usb;
/* You can create the instance of the class in two ways */
BTD Btd(&Usb); 
PS3BT PS3(&Btd);// This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printAngle;
uint8_t state = 0;

void setup() {
  pinMode(7,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  Serial.begin(9600);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
  xbee.begin(9600);
}

void loop() {
  Usb.Task();
   getControllerInputs();
  sendData();
  if(xbee.available()>=4){
    while(xbee.available())
      Serial.print(xbee.read()); 
    digitalWrite(4,HIGH);
    old_rx=millis();
  }
    if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
        if (PS3.getAnalogButton(L1) ) {
            xbee.print(F("\r\nL1: "));
            xbee.print(PS3.getAnalogButton(L1));
            xbee.print("\r");
      }
      else if (PS3.getAnalogButton(L2) ) {
            xbee.print(F("\r\nL2: "));
            xbee.print(PS3.getAnalogButton(L1));
            xbee.print("\r");
      }
      else if (PS3.getAnalogButton(R1) ) {
            xbee.print(F("\r\nR1: "));
            xbee.print(PS3.getAnalogButton(R1));
            xbee.print("\r");
      }

      else if (PS3.getAnalogButton(R2) ) {
            xbee.print(F("\r\nR2: "));
            xbee.print(PS3.getAnalogButton(L1));
            xbee.print("\r");
      }
      else if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 ){
            xbee.print(F("\r\nLHX: "));
            xbee.print(PS3.getAnalogHat(LeftHatX));
            xbee.print("\r");
            xbee.print(F("\r\nLHY: "));
            xbee.print(PS3.getAnalogHat(LeftHatY));
            xbee.print("\r");
      }

      else if (PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117 ){
            xbee.print(F("\r\nRHX: "));
            xbee.print(PS3.getAnalogHat(RightHatX));
            xbee.print("\r");
            xbee.print(F("\r\nRHY: "));
            xbee.print(PS3.getAnalogHat(RightHatY));
            xbee.print("\r");
      }

      else if (PS3.getButtonClick(TRIANGLE)){
          xbee.print(F("\r\nT:\r"));
        }
      else if (PS3.getButtonClick(SQUARE)){
          xbee.print(F("\r\nS:\r"));
        }
      else if (PS3.getButtonClick(CROSS)){
          xbee.print(F("\r\nC:\r"));
        }
      else if (PS3.getButtonClick(CIRCLE)){
          xbee.print(F("\r\nO:\r"));
        }  
    }


    else if (PS3.PS3MoveConnected) { // One can only set the color of the bulb, set the rumble, set and get the bluetooth address and calibrate the magnetometer via USB
  
    }
  
  }





void sendData(){
  if(millis()>=(old_tx+TX_INTERVAL)){
    xbee.write(filter,sizeof(filter));
    xbee.write(vals,sizeof(vals));
    xbee.write(filtere,sizeof(filtere));
    old_tx=millis();
  }
}

/*------------------------------------Main Functions-----------------------------------------*/
void getControllerInputs(){
  if(PS3.PS3Connected){ 
    digitalWrite(7,HIGH);
    calcPs3Input();
  }
  else{
    digitalWrite(7,LOW);
    vals[0]=127;
    vals[1]=0;
    vals[2]=0b01000000;
  }
}



void calcPs3Input(){
  pos=PS3.getAnalogHat(LeftHatX)-128;
  negOff=offset_ps3-deadzone_ps3;
  posOff=offset_ps3+deadzone_ps3;
  l2=PS3.getAnalogButton(L2);
  r2=PS3.getAnalogButton(R2);
  
  vals[0]=map(min(pos,negOff),negOff,-128,127,0)+map(max(pos,posOff),posOff,127,0,128); 

  if(l2>0){
    vals[1]=l2;
    vals[2]|=0b10000000;
  }

  
}
  

  



