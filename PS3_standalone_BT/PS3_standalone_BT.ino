/*
 Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3BT.h>
#include <PS3USB.h>
#include <usbhub.h>
#include <Servo.h>

int left_1 = 6;
int left_2 = 5;
int right_1 = 9;
int right_2 = 3;

Servo left_drive;
Servo right_drive;

Servo lift_actuator;
Servo tilt_bucket;
Servo back_hoe_x;
Servo back_hoe_y;


//Please enter these pins
int left_drive_pin = 5 ;
int right_drive_pin = 6;

int lift_actuator_pin;
int tilt_bucket_pin;

int back_hoe_x_pin;
int back_hoe_y_pin;


// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

//BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
//PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

PS3USB PS3(&Usb);


bool printTemperature, printAngle;

void init_actuators(){
  left_drive.attach(left_drive_pin);
  right_drive.attach(right_drive_pin);
  lift_actuator.attach(lift_actuator_pin);
  tilt_bucket.attach(tilt_bucket_pin);
  back_hoe_x.attach(back_hoe_x_pin);
  back_hoe_y.attach(back_hoe_y_pin);

  pinMode(left_1 ,OUTPUT);
  pinMode(left_2 ,OUTPUT);
  pinMode(right_1 ,OUTPUT);
  pinMode(right_2 ,OUTPUT);
  }

void setup() {
  init_actuators();
  
  Serial.begin(9600);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}


//Loop begins
void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    
    // Analog button values can be read from almost all buttons
    
    
    
    if ( PS3.getButtonPress(L2) || PS3.getButtonPress(R2)) {
      //int L_channel = PS3.getAnalogButton(L2);
      
      bool L1_Click = PS3.getButtonPress(L1);
      
      /////////////////////////////////////////////
      if (L1_Click )
        {
        left_drive.write(map(PS3.getAnalogButton(L2),127,255,0,255));
        //analogWrite(left_2,PS3.getAnalogButton(L2));
         //analogWrite(left_1,0);
         } 
      else {
          left_drive.write(map(PS3.getAnalogButton(L2),127,0,0,255));
          //analogWrite(left_1,PS3.getAnalogButton(L2));
          //analogWrite(left_2,0);
          Serial.print("L\r\n");
      }
      //int R_channel = PS3.getAnalogButton(R2);
      bool R1_Click = PS3.getButtonPress(R1);
      //////////////////////////////////////////////
      if (R1_Click){
            right_drive.write(map(PS3.getAnalogButton(L2),127,255,0,255));
            //analogWrite(right_1,0);
            //analogWrite(right_2,PS3.getAnalogButton(R2));
      }
      else {
          right_drive.write(map(PS3.getAnalogButton(L2),127,0,0,255));
          //analogWrite(right_1,PS3.getAnalogButton(R2));
          //analogWrite(right_2,0);
          Serial.print("R\r\n");
          
      }
    }
    else{
      analogWrite(left_2,0);
      analogWrite(left_1,0);
      analogWrite(right_2,0);
      analogWrite(right_1,0);
      }

  }

  else{
      analogWrite(left_2,0);
      analogWrite(left_1,0);
      analogWrite(right_2,0);
      analogWrite(right_1,0);
      }

}
