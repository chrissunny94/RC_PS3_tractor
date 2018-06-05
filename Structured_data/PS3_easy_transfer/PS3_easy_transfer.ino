
#include <EasyTransfer.h>
//#include <PS3BT.h>
#include <PS3USB.h>
USB Usb;
//BTD Btd(&Usb);

EasyTransfer ET;

//PS3BT PS3(&Btd);
PS3USB PS3(&Usb);
struct SEND_DATA_STRUCTURE{
  
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

//data group
SEND_DATA_STRUCTURE txdata;

void setup(){
 
 
  Serial.begin(115200);
 
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
 
  ET.begin(details(txdata), &Serial);
 
}

void loop(){
 
  Usb.Task();
 
  if(PS3.PS3Connected || PS3.PS3NavigationConnected)
  { 
   txdata.l1 = PS3.getButtonPress(L1);
   txdata.l2 = PS3.getAnalogButton(L2);
   txdata.r1 = PS3.getButtonPress(R1);
   txdata.r2 = PS3.getAnalogButton(R2);
   txdata.lx = PS3.getAnalogHat(LeftHatX);
   txdata.ly = PS3.getAnalogHat(LeftHatY);        
   txdata.rx = PS3.getAnalogHat(RightHatX);
   txdata.ry = PS3.getAnalogHat(RightHatY);
   txdata.x = PS3.getButtonPress(CROSS);
   txdata.c = PS3.getButtonPress(CIRCLE);
   txdata.t = PS3.getButtonPress(TRIANGLE);
   txdata.s = PS3.getButtonPress(SQUARE);
   txdata.up = PS3.getButtonPress(UP);
   txdata.down = PS3.getButtonPress(DOWN);
   txdata.left = PS3.getButtonPress(LEFT);
   txdata.right = PS3.getButtonPress(RIGHT);
           
          
   ET.sendData();
  }
  
 
}

