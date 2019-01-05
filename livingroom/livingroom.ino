#include <Zchaos.h>
#include <Zir.h>
// #include <Zcommand.h>
#include <Zsmonoff.h>
#include "Zledcontroller.h"

#define IRREC     3
#define LAMP1     7    
#define LAMP2     5    
#define PRINTER1  6    
#define LAMP1DIMM 9

Zir ir(IRREC);
// Zcommand command;
Zsmonoff lamp2(LAMP2);
Zsmonoff printer1(PRINTER1);
int dimpos = 0;

Zledcontroller led(LAMP1DIMM ,LAMP1);

void vol_up(bool _long)    
{
  if (dimpos < dim_steps_size) dimpos ++;
  led.set(dim_steps[dimpos]);
}


void vol_down(bool _long)    
{
  if (dimpos > 0) dimpos--;
  led.set(dim_steps[dimpos]);
}


void lamp1switch(bool _long)    
{ 
  if (Zledcontroller::State::ON  == led.getState()) dimpos = 0; 
  if (Zledcontroller::State::OFF == led.getState()) dimpos = dim_steps_size; 
  led.set(dim_steps[dimpos]);
}


void lamp2switch(bool _long)    { lamp2.switching(); }
void printer1switch(bool _long) { printer1.switching(); }
void eventPower(bool _long)     { lamp1switch(_long ); }
void lamp1on(bool _long)    { led.set(255); }
void lamp2on(bool _long)    { lamp2.on(); }
void printer1on(bool _long) { printer1.on(); }
void lamp1off(bool _long)    { led.set(0); }
void lamp2off(bool _long)    { lamp2.off(); }
void printer1off(bool _long) { printer1.off(); }


void setup() 
{
  Serial.begin(9600);
  Serial.println("Start");
  ir.setup();
  printer1.setup();
  led.setup();
  pinMode(LAMP1, OUTPUT);
  ir.registerHandler(IR_REMOTE_CODE_VOL_UP, vol_up, true);
  ir.registerHandler(IR_REMOTE_CODE_VOL_DOWN, vol_down, true);
  ir.registerHandler(IR_REMOTE_CODE_POWER, eventPower, false);
  ir.registerHandler(IR_REMOTE_CODE_1, lamp1switch, false);
  ir.registerHandler(16718055, lamp2switch, false);
  ir.registerHandler(16732845, printer1switch, false);
  ir.registerHandler(1099325599, lamp1switch, false);
  ir.registerHandler(49872     , lamp2switch, false);
  digitalWrite(LAMP1, LOW);

}


void loop() 
{
  ir.loop();
  led.loop();
}
