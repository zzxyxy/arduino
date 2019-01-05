#include "Zledcontroller.h"
#include <Arduino.h>

#define NOT_USED -1

Zledcontroller::Zledcontroller(const int dimPin)
  :mDimPin(dimPin),mSwitchPin(NOT_USED) { }


Zledcontroller::Zledcontroller(const int dimPin, const int switchPin)
  :mDimPin(dimPin),mSwitchPin(switchPin) { }

void Zledcontroller::setup()
{
  pinMode(mDimPin, OUTPUT);
  analogWrite(mDimPin,0);
  if (mSwitchPin!=NOT_USED) {
    pinMode(mSwitchPin, OUTPUT);
    digitalWrite(mSwitchPin,HIGH);    
  } 
}

void Zledcontroller::loop()
{
  if (setDim == actualDim) return;

  if (mSwitchPin!=NOT_USED) {
    if (!mSwitch && (0 < setDim)) {
      Serial.println("set");
      mSwitch = true;
      digitalWrite(mSwitchPin,LOW); 
    }
  }

  if ((timestamp+wait < millis() )) {
    timestamp = millis();
    if (setDim > actualDim) {
      ++actualDim;
      wait = 40;
    } else {
      --actualDim;
      wait = 10;
    }
    analogWrite(mDimPin,actualDim);
  }
    
  if (mSwitchPin!=NOT_USED) {
    if (mSwitch && (0 == actualDim)) {
      mSwitch = false;
      digitalWrite(mSwitchPin,HIGH); 
    }
  }
}


void Zledcontroller::set(int dim)
{
  setDim = dim;
}


Zledcontroller::State Zledcontroller::getState() const
{
  return 0==setDim ? Zledcontroller::State::OFF : Zledcontroller::State::ON;
}