# uLCD-32-PTU-example
This is a uLCD-32-PTU-example code and tutorial

Materials used:
* Arduino Uno
* uLCD-32PTU-AR
* 4D Arduino Adaptor Shield
* 5 pin ribbon cable (Included in 4D Arduino Adaptor Shield)
* SD Card PC adapter
* SD (SDHC) 2-32GB Memory
* µUSB-PA5 - USB to Serial Bridge

To connect uLCD-32PTU-AR display to arduino do the following:

1. Connect 6 pin cable to touch screen back side as seen in the picture

![4D Touch Screen connected to Arduino via Serial](https://github.com/Kaibots/uLCD-32-PTU-example/blob/master/images/uLCD-32PTU-AR%20Screen%20with%20cable.jpeg?raw=true "4D Systems Touch Screen with Serial cable")

1. Connect the other end of the cable to the right side of the 4D Arduino Adaptor Shield

![Arduino with shield and Serial 6 pin cable](https://github.com/Kaibots/uLCD-32-PTU-example/blob/master/images/uLCD-32PTU-AR%20Screen%20with%20cable.jpeg?raw=true "arduino with 4d shield and serial 6 pin cable")

1. We will not be using the Pin 2 of the Danger Shield since that’s used as a Reset Pin for the touch screen

![Danger Shield Back with unused pin 2](https://github.com/Kaibots/uLCD-32-PTU-example/blob/master/images/danger_shield_back.jpeg?raw=true "Danger Shield back side")

1. Connect the Danger shield on top of the Stack

![Danger shield](https://github.com/Kaibots/uLCD-32-PTU-example/blob/master/images/danger_shield.jpeg?raw=true "Danger shield")
![Danger shield stack](https://github.com/Kaibots/uLCD-32-PTU-example/blob/master/images/shields_stack.jpeg?raw=true "Danger shield and 4d stack")

1. Download Workshop 4 from [this page](http://www.4dsystems.com.au/product/4D_Workshop_4_IDE/)

1. Open Workshop 4

1. Select New..

1. Find uLCD-32PTU WITHOUT the “-AR” at the end since picking the screen with -AR will not allow you to use the ViSi-Genie interface we used.

1. Select the Visi-Genie Environment 

1. Create this interface in ViSi-Genie

![Visi-Genie User Interface](https://github.com/Kaibots/uLCD-32-PTU-example/blob/master/images/Workshop4-UI.jpg?raw=true "workshop 4 visi genie ui")
 
Using:
* Gauges - Gauge
* Digits - LED Digits
* Inputs - Dip Switch
* Inputs - Rocker
* Digits - User LED


1. After all this is setup, connect the touch screen to the PC using an USB to USB mini cable and the uUSB-PA5 bridge to your computer.



1. Connect the SD Card adapter and SD Card to your computer

1. Build and Load the Workshop 4 interface to the SD Card

1 .Connect the Arduino to your PC and load this code into it

```
#include <genieArduino.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

const byte ledCharSet[10] = {
  B00111111, B00000110, B01011011, B01001111, B01100110, B01101101, B01111101, B0000011, B01111111, B01101111  
};

#ifndef TRUE
#define TRUE (1==1)
#define FALSE (!TRUE)
#endif

#define LATCH 7
#define CLOCK 8
#define DATA 4

#define RESET_LINE 4

int slider1;
int slider2;
int slider3;
int button1;
int button2;
int button3;
int led1;
int led2;
int temp;
int light;
int digit = 0;
int lastdigit = 1;

long nextUpdate = 0;

Genie genie;

void setup() {
  pinMode (13, OUTPUT);
  digitalWrite(13, 0);
  
  Serial.begin(115200);
  genie.Begin(Serial);

  genie.AttachEventHandler(myGenieEventHandler);

  pinMode(10, INPUT);
  digitalWrite(10, HIGH);
  pinMode(11, INPUT);
  digitalWrite(11, HIGH);
  pinMode(12, INPUT);
  digitalWrite(12, HIGH);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);

  pinMode(RESET_LINE, OUTPUT);
  digitalWrite(RESET_LINE, 1);
  delay(100);
  digitalWrite(RESET_LINE, 0);

  delay (3500);
  
  genie.WriteStr(0, GENIE_VERSION);
}

  void loop() {
  while(1){
    genie.DoEvents();
    if(millis() >= nextUpdate){        
      dangerShield();
      lcdUpdate();
      nextUpdate = millis() + 50;
    }  
  }
}

void dangerShield() {
  slider1 = map(analogRead(A2), 0, 1024, 1024, 0);
  slider2 = map(analogRead(A1), 0, 1024, 1024, 0);
  slider3 = map(analogRead(A0), 0, 1024, 1024, 0);
  light = map(analogRead(A3), 0, 1024, 0, 100);
  digit = map(slider1, 0, 1024, 0, 9);  
}
void lcdUpdate() {
  genie.WriteObject (GENIE_OBJ_GAUGE, 0, slider1);
  genie.WriteObject (GENIE_OBJ_GAUGE, 1, slider2);
  genie.WriteObject (GENIE_OBJ_GAUGE, 2, slider3);
  genie.WriteObject (GENIE_OBJ_GAUGE, 3, light);
  genie.WriteObject (GENIE_OBJ_LED_DIGITS, 0, digit);
// 
  genie.ReadObject (GENIE_OBJ_ROCKERSW, 0);
  genie.ReadObject (GENIE_OBJ_ROCKERSW, 1);
}

void myGenieEventHandler(void) {
  genieFrame Event;
  genie.DequeueEvent(&Event);
  if(Event.reportObject.cmd == GENIE_REPORT_OBJ){
    if(Event.reportObject.object == GENIE_OBJ_ROCKERSW) {
      if(Event.reportObject.index == 0){
        bool switch0 = genie.GetEventData(&Event);
        if(switch0 == 1) {
          digitalWrite(5, HIGH);  
        } else {
          digitalWrite(5 ,LOW);  
        }
      }
      if(Event.reportObject.index == 1){
        bool switch1 = genie.GetEventData(&Event);
        if(switch1 == 1) { 
          digitalWrite(6, HIGH);  
        } else {
          digitalWrite(6, LOW);  
        }
      }
    }  
  }
}
```

1. Connect the 4D Arduino Adaptor Shield to the uLDC screen
Note: In order to properly download the code into the Arduino the uLCD Screen has to be disconnected because it uses the same pins to communicate to the Arduino via Serial and it interferes with the code being uploaded to the ATMEGA.

11. You should the the value of the Sliders shown in the screen’s gauges and the LEDs should turn ON and OFF after toggling the Rocker switches in the touch screen, also the Light Sensor displays its value in one of the Gauges
