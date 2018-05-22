#include <PS3USB.h>

#include <SoftwareSerial.h>

SoftwareSerial xbee(10, 11); // RX, TX

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printAngle;
uint8_t state = 0;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
  xbee.begin(115200);
}

void loop() {
  Usb.Task();
    if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
        if (PS3.getAnalogButton(L1) ) {
            xbee.print(F("\r\nL1: "));
            xbee.print(PS3.getAnalogButton(L1))
      }
      else if (PS3.getAnalogButton(L2) ) {
            xbee.print(F("\r\nL2: "));
            xbee.print(PS3.getAnalogButton(L1))
      }
      else if (PS3.getAnalogButton(R1) ) {
            xbee.print(F("\r\nL1: "));
            xbee.print(PS3.getAnalogButton(L1))
      }

      else if (PS3.getAnalogButton(R2) ) {
            xbee.print(F("\r\nL1: "));
            xbee.print(PS3.getAnalogButton(L1))
      }
    }


    else if (PS3.PS3MoveConnected) { // One can only set the color of the bulb, set the rumble, set and get the bluetooth address and calibrate the magnetometer via USB
  
    }
  
  }

  



