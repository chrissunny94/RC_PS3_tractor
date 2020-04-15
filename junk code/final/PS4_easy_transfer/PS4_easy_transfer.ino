
#include <EasyTransfer.h>
#include <PS4BT.h>
//#include <PS4USB.h>
USB Usb;
BTD Btd(&Usb);

EasyTransfer ET;

PS4BT PS4(&Btd,PAIR);
//PS4USB PS4(&Usb);
struct SEND_DATA_STRUCTURE{
  
  int count;
  int left;
  int right;
  
  
};

#define TX_INTERVAL 5
#define RX_TIMEOUT 100
#define STATUS_BLINKRATE 250

//data group
SEND_DATA_STRUCTURE txdata;

void setup(){
 
 
  Serial.begin(115200);
 
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
 
  ET.begin(details(txdata), &Serial);
  
 
}

void loop(){
 
  Usb.Task();
 
  if(PS4.connected() )
  { 
   calcPS4Input();
  }

  else{
    txdata.count = 0;
    txdata.left=0;
    txdata.right =0;

    sendData();
    }
 
}

long l2, r2;
double old_tx;
double old_rx;
double old_blink;

void sendData(){
  if(millis()>=(old_tx+TX_INTERVAL)){
    
      ET.sendData();
      old_tx=millis();
  }
}
int counter = 0;
void calcPS4Input(){
  if (PS4.getButtonPress(L2) || PS4.getButtonPress(R2) || PS4.getButtonClick(L1)|| PS4.getButtonClick(R1)|| PS4.getButtonClick(L2)|| PS4.getButtonClick(R2)){
    bool l1_press = PS4.getButtonPress(L1);
   txdata.count=0;
    l2=PS4.getAnalogButton(L2);
    if(l1_press)
      txdata.left= (-1*(l2));
    else
      txdata.left= ((l2));
  
  
  
    bool r2_press = PS4.getButtonPress(R1);
    
    r2=PS4.getAnalogButton(R2);
    if(r2_press)
      txdata.right= (-1*(r2));
    else
      txdata.right= (r2);

      
      sendData();
 

}
else if (++counter >10){
  txdata.count =0;
  txdata.left=0;
  txdata.right=0;
  sendData();
  counter =0;
  }
   
  if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 ){
    txdata.count=1;
    txdata.left= (PS4.getAnalogHat(LeftHatX));
    txdata.right= (PS4.getAnalogHat(LeftHatY));
    sendData();
    
    
  }
  

  if (PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117){
    
    txdata.count=2;
    txdata.left= (PS4.getAnalogHat(RightHatX));
    txdata.right= (PS4.getAnalogHat(RightHatY));
    sendData();
  }
   
  if(PS4.getButtonPress(UP)||PS4.getButtonPress(DOWN)){
    txdata.count=3;
    txdata.left= (PS4.getButtonPress(UP));
    txdata.right= (PS4.getButtonPress(DOWN));
    sendData();
    }

  if(PS4.getButtonPress(LEFT)||PS4.getButtonPress(RIGHT)){
    txdata.count=4;
    txdata.left= (PS4.getButtonPress(LEFT));
    txdata.right= (PS4.getButtonPress(RIGHT));
    sendData();
    }
 
   if(PS4.getButtonPress(CROSS)||PS4.getButtonPress(SQUARE)){
    
    txdata.count=5;
    txdata.left= (PS4.getButtonPress(SQUARE));
    txdata.right=(PS4.getButtonPress(CROSS));
    sendData();
    }
 
  if (PS4.getButtonPress(TRIANGLE)||PS4.getButtonPress(CIRCLE)){
    txdata.count=6;
    txdata.left= (PS4.getButtonPress(CIRCLE));
    txdata.right=(PS4.getButtonPress(TRIANGLE));
    sendData();
    }
   
  
}
