#include <Adafruit_PWMServoDriver.h>

#include "Zir.h"
#include "Zchaos.h"
#include "Zsmonoff.h"
#include "Zserial.h"

Adafruit_PWMServoDriver dim = Adafruit_PWMServoDriver();
Zir ir(4);
Zserial zser;
Zsmonoff relay1(6);
Zsmonoff relay2(7);
Zsmonoff relay3(8);
Zsmonoff relay4(9);

int dimstate = 0;

void setdim(int newd)
{
  if ((dimstate == 0) && (newd > 0)) {
    relay1.on();
  }
  if ((dimstate > 0) && (newd == 0)) {
    relay1.off();
  }
  dim.setPWM(0, 0, newd);   
  dimstate = newd;
}



void dim_up() 
{
  if (dimstate == 0) {
    setdim(1);
    return;
  }

  if (dimstate < 5) {
    setdim(dimstate+1); 
  }
  const int newstate = (int) ((double) dimstate * 1.2d);
  if (newstate > 4095) { 
    setdim(4095);
    return;
  }
  setdim(newstate);
}


void dim_down() 
{
  if (dimstate < 5) {
    setdim(dimstate-1); 
  }
  const int newstate = (int) ((double) dimstate * .8d);
  if (newstate < 2) { 
    setdim(0);
    return;
  }
  setdim(newstate);
}


void ledwallswitch()
{
  if (relay1.getState() == Zsmonoff::State::ON)
  {
    setdim(0);
  } else {
    setdim(4095);
  }
}


void balllampswitch()
{
  relay3.switching();
}


void setup() 
{

  dim.begin();
  dim.setPWMFreq(120);  // Analog servos run at ~60 Hz updates

  zser.setup();
  relay1.setup();
  relay2.setup();
  relay3.setup();
  relay4.setup();
  ir.setup();

  ir.registerHandler(IR_REMOTE_CODE_POWER, ledwallswitch, false);
  ir.registerHandler(IR_REMOTE_ER_POWER_TV, ledwallswitch, false);
  ir.registerHandler(IR_REMOTE_CODE_1, ledwallswitch, false);
  ir.registerHandler(IR_REMOTE_CODE_2, balllampswitch, false);
  ir.registerHandler(IR_REMOTE_ER_POWER_STB, balllampswitch, false);

  ir.registerHandler(IR_REMOTE_CODE_VOL_UP, dim_up, true);
  ir.registerHandler(IR_REMOTE_CODE_VOL_DOWN, dim_down, true);

  ir.registerHandler(IR_REMOTE_ER_TV_VOL_UP, dim_up, true);
  ir.registerHandler(IR_REMOTE_ER_TV_VOL_DOWN, dim_down, true);

  zser.registerCommand(COMMAND_POWER_SWITCH, ledwallswitch);
  zser.registerCommand(COMMAND_SET1, ledwallswitch);
  zser.registerCommand(COMMAND_SET2, balllampswitch);

   
}

void loop() 
{
  ir.loop();
  zser.loop();
}
