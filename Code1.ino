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
