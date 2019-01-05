#define RELAY1  4    

#include <Zir.h>
#include <Zcommand.h>
#include <Zsmonoff.h>

Zir ir(3);
Zcommand command;
Zsmonoff relay1(RELAY1);

void testf() 
{
  relay1.switching();
}

void setup() 
{
  const char testCmd[] = "test";
  
  Serial.begin(9600);
  Serial.println("Start");
  ir.setup();
  ir.registerHandler(16753245, testf, false);
  relay1.setup();
  command.registerCommand(testCmd, 4, testf );
}

void loop() 
{
  ir.loop();

  if (Serial.available() > 0) command.serialCommand(Serial.read());
  
}



