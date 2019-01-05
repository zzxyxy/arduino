#undef DEBUG

#include "Zchaos.h"
#include "Zserial.h"

#define S_INIT        0x0000
#define S_BRIGHT      0x0001
#define S_DARK        0x0002
#define S_ON          0x0003
#define S_OFF         0x0004
#define S_RESETTIMER  0x0005
#define S_TIMER       0x0006

#define E_DARK        0x1001
#define E_LIGHT       0x1002
#define E_MOVE        0x1003
#define E_TURNON      0x1004
#define E_RESETTIMER  0x1005
#define E_TIMER       0x1006
#define E_TURNOFF     0x1007
#define E_LAMPOFF     0x1008

#define DARK_         40
#define BRIGHT_       60

int state = S_INIT;

int ledPin = 13;  // LED on Pin 13 of Arduino
int lampPin = 11;
int pirPin = 7; // Input for HC-S501
int lampB = 0;
bool lampState = false;
int pirValue; // Place to store read PIR Value
int LDRValue = 0; 
unsigned long timer = -1;
unsigned long now = 0;

Zserial zser;

bool dark = false;

void event(int event) 
{
  switch (state) {    
    case S_INIT:
      switch(event) {
        case(E_DARK):
          state=S_OFF;
        break;
        case(E_LIGHT):
          state=S_BRIGHT;
        break;
      }
    break;
    case S_BRIGHT:
      switch(event) {
        case(E_DARK):
          state=S_OFF;
        break;
      } 
    break;    
    case S_OFF:
      switch(event) {
        case(E_MOVE):
          state = S_ON;
        break;
        case(E_LAMPOFF):
          if (!dark) state = S_BRIGHT;
        break;
      }
    break;    
    case S_ON:
      switch(event) {
        case(E_TIMER):
          state = S_OFF;
        break;
        case(E_MOVE):
      timer = now + 1000;
        break;
      }
    break;    
  }

  switch (state) {
    case S_BRIGHT:
      if (lampState) lampState = false;
      if ((DARK_ > LDRValue) && (!dark)) {
        dark = true;
        state=S_OFF;
      }
    break;    
    case S_OFF:
      if (lampState == true) lampState = false;
    break;    
    case S_ON:
      timer = now + 2000;
      if (lampState == false) lampState = true;
    break;    
  }
}


void check_dark() {
  if ((DARK_ > LDRValue) && (!dark)) {
    dark = true;
    event(E_DARK);
  }
  else if ((BRIGHT_ < LDRValue) && (dark)) {
    dark = false;
    event(E_LIGHT);
  }
}

int lastpir=0;


void check_timer() {
  if (now > timer) {  
    event(E_TIMER);
    timer = -1;
  }
}


void check_move() {
  pirValue = digitalRead(pirPin);
  digitalWrite(ledPin, pirValue);
  if (1 == pirValue) {
    lastpir=1;
    event(E_MOVE);
  } 
}

void setup() {
  zser.setup();
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(lampPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  analogWrite(lampPin, 0);
  LDRValue = analogRead(0);  
  check_dark();  
}

void loop() {
  now = millis();
  LDRValue = analogRead(0);
  check_move();
  check_timer();
  check_dark();
  setLamp();
  delay(25);
}

unsigned int lampdelay=0;

void setLamp() {
  if (now < lampdelay) return;
  
  if (lampState) {
    lampdelay = now + 20;
    if (lampB < 255) {
      lampB++;
      analogWrite(lampPin, lampB);  
    }
  } else {
    if (lampB > 80) lampdelay = now + 30;
    else lampdelay = now + 100;
    
    if (lampB > 0) {
      lampB--;
      if (lampB < 1) event(E_LAMPOFF);
      analogWrite(lampPin, lampB);  
    }    
  }
}


