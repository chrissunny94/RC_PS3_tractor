#include <PS3BT.h>
#include <XBOXUSB.h>

#define TX_INTERVAL 5
#define RX_TIMEOUT 100
#define STATUS_BLINKRATE 250

#define deadzone_xbox 15
#define offset_xbox 7
#define deadzone_ps3 7
#define offset_ps3 2

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

byte filter[] ={0b10101010,0b01100110,0b10101010};
byte filtere[]={0b11111110,0b01111110,0b01111111};
byte vals[]={127,0,0b01000000};
int pos, negOff, posOff, l2, r2;

int speed_state=0;

double old_tx;
double old_rx;
double old_blink;

double avg=4;
boolean lowbat=false;

int i=0;

/*----------------------------------------Main loop--------------------------------------*/

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(7,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  if (Usb.Init() == -1){
    while(1){
      Serial.println("USB failed");
      delay(500);
    }
  }
  analogReadResolution(16);
}

void loop() {
  blinkStatus();
  Usb.Task();
  getControllerInputs();
  sendData();
  if(Serial1.available()>=4){
    while(Serial1.available())
      Serial.print(Serial1.read()); 
    digitalWrite(4,HIGH);
    old_rx=millis();
  }
  if(millis()>old_rx+RX_TIMEOUT)
     digitalWrite(4,LOW);
     
  avg-=(avg-((analogRead(A0)/65535.0)*3.3+1.11))/50;
  if(avg<=3.6)
    lowbat=true;
  if(avg<=3.65)
    lowbat=false;
  Serial.println(avg);
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

void sendData(){
  if(millis()>=(old_tx+TX_INTERVAL)){
    Serial1.write(filter,sizeof(filter));
    Serial1.write(vals,sizeof(vals));
    Serial1.write(filtere,sizeof(filtere));
    old_tx=millis();
  }
}

/*-------------------------------------Sub functions--------------------------------------*/
void blinkStatus(){
   if(millis()>old_blink+STATUS_BLINKRATE){
     if(!lowbat)
       digitalWrite(3,!digitalRead(3));
     else
       digitalWrite(3,HIGH);
     old_blink=millis();
   }
}

void calcPs3Input(){
  pos=PS3.getAnalogHat(LeftHatX)-128;
  negOff=offset_ps3-deadzone_ps3;
  posOff=offset_ps3+deadzone_ps3;
  l2=PS3.getAnalogButton(L2);
  r2=PS3.getAnalogButton(R2);
  if(PS3.getButtonClick(START)&&PS3.getButtonPress(L1)&&PS3.getButtonPress(R1)){
     speed_state++;
     if(speed_state==3)
       speed_state=0;
      
     PS3.setLedOff(LED3);
     PS3.setLedOff(LED4);  
     if(speed_state==1)
       PS3.setLedOn(LED4);
     else if(speed_state==2){
       PS3.setLedOn(LED3);
       PS3.setLedOn(LED4);
     }
     
  }
  
  vals[0]=map(min(pos,negOff),negOff,-128,127,0)+map(max(pos,posOff),posOff,127,0,128); 

  if(l2>0){
    vals[1]=l2;
    vals[2]|=0b10000000;
  }
  else{
    switch(speed_state){
       case 0:
         vals[1]=r2*0.50;
         break;
       case 1:
         vals[1]=r2*((PS3.getButtonPress(TRIANGLE))? 1:0.70);
         break;
       case 2:
         vals[1]=r2;
         break; 
    }
    vals[2]&=0b01111111;
  }
  if(PS3.getButtonPress(CIRCLE))
    vals[2]|=0b01000000;
  else
    vals[2]&=0b10111111;
}

