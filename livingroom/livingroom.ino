#include <Zir.h>
#include <Zcommand.h>
#include <Zsmonoff.h>


#define IRREC     3
#define LAMP1     4    
#define LAMP2     5    
#define PRINTER1  6    


Zir ir(IRREC);
Zcommand command;
Zsmonoff lamp1(LAMP1);
Zsmonoff lamp2(LAMP2);
Zsmonoff printer1(PRINTER1);


void lamp1switch(bool _long)    { lamp1.switching(); }
void lamp2switch(bool _long)    { lamp2.switching(); }
void printer1switch(bool _long) { printer1.switching(); }
void eventPower(bool _long)     { lamp1switch(_long ); }
void lamp1on(bool _long)    { lamp1.on(); }
void lamp2on(bool _long)    { lamp2.on(); }
void printer1on(bool _long) { printer1.on(); }
void lamp1off(bool _long)    { lamp1.off(); }
void lamp2off(bool _long)    { lamp2.off(); }
void printer1off(bool _long) { printer1.off(); }

const char lamp1Cmd[] = "000001tog";
const char lamp2Cmd[] = "000002tog";
const char printer1Cmd[] = "000100tog";
const char lamp1On[] = "000001on";
const char lamp2On[] = "000002on";
const char printer1On[] = "000100on";
const char lamp1Off[] = "000001off";
const char lamp2Off[] = "000002off";
const char printer1Off[] = "000100off";


void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  ir.setup();
  printer1.setup();
  lamp1.setup();
  lamp2.setup();
  command.registerCommand(lamp1Cmd, 9, lamp1switch );
  command.registerCommand(lamp2Cmd, 9, lamp2switch );
  command.registerCommand(printer1Cmd, 9, printer1switch);

  command.registerCommand(lamp1On, 8, lamp1on );
  command.registerCommand(lamp2On, 8, lamp2on );
  command.registerCommand(printer1On, 8, printer1on);

  command.registerCommand(lamp1Off, 9, lamp1off );
  command.registerCommand(lamp2Off, 9, lamp2off );
  command.registerCommand(printer1Off, 9, printer1off);

  ir.registerHandler(16753245, eventPower, false);
//  ir.registerHandler(16738455, stateC0, false);
  ir.registerHandler(16724175, lamp1switch, false);
  ir.registerHandler(16718055, lamp2switch, false);
//  ir.registerHandler(16743045, stateC3, false);
//  ir.registerHandler(16716015, stateC4, false);
//  ir.registerHandler(16726215, stateC5, false);
//  ir.registerHandler(16734885, stateC6, false);
//  ir.registerHandler(16728765, stateC7, false);
//  ir.registerHandler(16730805, stateC8, false);
  ir.registerHandler(16732845, printer1switch, false);
  ir.registerHandler(1099325599, lamp1switch, false);
  ir.registerHandler(49872     , lamp2switch, false);
}


void loop() {
  ir.loop();
  if (Serial.available() > 0) command.serialCommand(Serial.read());
}
