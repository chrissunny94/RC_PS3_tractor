#include <PS3BT.h>
//#include <PS3USB.h>


#define TX_INTERVAL 5
#define RX_TIMEOUT 100
#define STATUS_BLINKRATE 250


USB Usb;
//PS3USB PS3(&Usb);
BTD Btd(&Usb);
PS3BT PS3(&Btd);


long l2, r2;



double old_tx;
double old_rx;
double old_blink;

double avg=4;


int i=0;
String output;

/*----------------------------------------Main loop--------------------------------------*/

void setup() {
  Serial.begin(115200);
  Serial.print("PS3 Module started");
  
  
  if (Usb.Init() == -1){
    while(1){
      Serial.println("USB failed");
      delay(500);
    }
  }
  else {
    Serial.println("\nPS3 detected vai USB");}
 
}

void loop() {
  
  Usb.Task();
  
  calcPs3Input();
  
  
}

/*------------------------------------Main Functions-----------------------------------------*/
void getControllerInputs_send(){
  if(PS3.PS3Connected){ 
    
    
    sendData();
  }
  else{
    digitalWrite(7,LOW);
    
  }
}



void sendData(){
  if(millis()>=(old_tx+TX_INTERVAL)){
    if(output.length()>3)
      writeString(output);
      old_tx=millis();
  }
}

/*-------------------------------------Sub functions--------------------------------------*/

void writeString(String stringData) { // Used to serially push out a String with Serial.write()

  for (int i = 0; i < stringData.length(); i++)
  {
    Serial.write(stringData[i]);   // Push each char 1 by 1 on each loop pass
  }

}// end writeString 

void calcPs3Input(){
  output = "";
  
  
  
  
  
  
  if (PS3.getButtonPress(L2) || PS3.getButtonPress(R2)){
    bool l1_press = PS3.getButtonPress(L1);
    output = "L:";
    l2=PS3.getAnalogButton(L2);
    if(l1_press)
      output += String(-1*(l2));
    else
      output += String((l2));
  
  
  
    bool r2_press = PS3.getButtonPress(R1);
    output += " R";
    r2=PS3.getAnalogButton(R2);
    if(r2_press)
      output += String(-1*(r2));
    else
      output += String(r2);

      output += "\n\r";
      getControllerInputs_send();
  }
   output = "";
  if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 ){
    output = "LX:"; 
    output += String(PS3.getAnalogHat(LeftHatX));
    output += " LY:"; 
    output += String(PS3.getAnalogHat(LeftHatY));
    output += "\n\r";
    getControllerInputs_send();
    
    
  }
   output = "";

  if (PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117){
    
    output = "RX:"; 
    output += String(PS3.getAnalogHat(RightHatX));
    output += " RY:"; 
    output += String(PS3.getAnalogHat(RightHatY));
    output += "\n\r";
    getControllerInputs_send();
  }
   output = "";
  if(PS3.getButtonPress(UP)||PS3.getButtonPress(DOWN)||PS3.getButtonPress(LEFT)||PS3.getButtonPress(RIGHT)){
    output  = "U:" + String(PS3.getButtonPress(UP));
    output += " D:" + String(PS3.getButtonPress(DOWN));
    output += " LE:" + String(PS3.getButtonPress(LEFT));
    output += " RI:" + String(PS3.getButtonPress(RIGHT));
    output += "\n\r";
    getControllerInputs_send();
    }
 output = "";
   if(PS3.getButtonPress(CROSS)||PS3.getButtonPress(SQUARE)||PS3.getButtonPress(TRIANGLE)||PS3.getButtonPress(CIRCLE)){
    output   = "C:" + String(PS3.getButtonPress(CIRCLE));
    output  += " T:" + String(PS3.getButtonPress(TRIANGLE));
    output  += " S:" + String(PS3.getButtonPress(SQUARE));
    output  += " X:" + String(PS3.getButtonPress(CROSS));
    output  += "\n\r";
    getControllerInputs_send();
    }
 
  
   
  
}




