#include "Zir.h"
#include "Zchaos.h"
#include "Zsmonoff.h"
#include "Zserial.h"

Zir ir(4);
Zserial zser;
Zsmonoff relay1(6);
Zsmonoff relay2(7);
Zsmonoff relay3(8);
Zsmonoff relay4(9);

void ledwallswitch()
{
  relay1.switching();
}


void balllampswitch()
{
  relay2.switching();
}


void setup() 
{
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

  zser.registerCommand(COMMAND_POWER_SWITCH, ledwallswitch);
  zser.registerCommand(COMMAND_SET1, ledwallswitch);
  zser.registerCommand(COMMAND_SET2, balllampswitch);

}

void loop() 
{
  ir.loop();
  zser.loop();
}
