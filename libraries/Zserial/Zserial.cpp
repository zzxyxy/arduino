#include "Zserial.h"

void Zserial::setup() 
{
  Serial.begin(9600);
}


void Zserial::loop()
{
  if (progress) {
    if ((timestamp + WAIT) < millis()) {
      reset();
    }
  }

  if (Serial.available()) 
  {
    int inp = Serial.read();
    if ((inp == 0xD) || (inp == 0xA)) return;

    progress = true;
    timestamp = millis();

    if (shift != 0x1) {
      if (param) {
        parameter = parameter + shift * inp;
        shift = shift / 0x100;
      } else {
        comm = comm + shift * inp;
        shift = shift / 0x100;
      }
    } else {
      if (param) {
        parameter = parameter + inp;
        run(comm, parameter);
        reset();
        param = false;
      } else {
        comm = comm + inp;
        shift = 0x1000000;   
        param = true;
      }
    }
  }
}


void Zserial::registerCommand(long code, void (*f)(long)) 
{
  m_list[m_regCounter].code = code;
  m_list[m_regCounter].f = &(*f);
  ++m_regCounter;
}


void Zserial::run(long command, long parameter) 
{
  for (int i=0; i<m_regCounter; ++i) {
	if (m_list[i].code == command) {
		(*m_list[i].f)(parameter);
		return;
	}
  }
}


void Zserial::reset()
{
  param = false;
  progress = false;
  parameter = 0;    
  comm = 0;     
  shift = 0x1000000;  
}

/*
        Serial.print("comm:");
        Serial.print(comm, HEX);
        Serial.println();
        Serial.print("param:");
        Serial.print(parameter, HEX);
        Serial.println();
*/
