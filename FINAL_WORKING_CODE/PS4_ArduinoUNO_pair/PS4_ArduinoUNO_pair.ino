
#include <EasyTransfer.h>
//#include <PS4BT.h>

#include <PS4USB.h>
USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);



void setup(){
 
 
  Serial.begin(115200);
 
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
 
  
 
}

void loop(){
 
  Usb.Task();
 
  if(PS4.connected() )
  { 
   calcPS4Input();
  }

   
  
}
