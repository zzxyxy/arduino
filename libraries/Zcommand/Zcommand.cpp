#include "Zcommand.h"


Zcommand::Zcommand()
{
}


Zcommand::Zcommand(const Zcommand& orig)
{
}


Zcommand::~Zcommand()
{
}


void Zcommand::registerCommand(char* command, int commandlength, void (*f)()) 
{
  m_list[m_regCounter].command = command;
  m_list[m_regCounter].f = &(*f);
  m_list[m_regCounter].commandlength = commandlength;
  ++m_regCounter;
}


void Zcommand::serialCommand(char in) {
	if ('\n' == in || '\0' == in ) {
		run();
		serialcmdpoint=0;
		
		return;
	}
	
	if (serialcmdpoint >= MAXSERIALCOMMANDLENGTH) return;
	serialcmd[serialcmdpoint] = in;
	++serialcmdpoint;
	serialcmd[serialcmdpoint] = "\0";
}


char* Zcommand::getLastCommand() 
{
	return serialcmd;
}


void Zcommand::run() 
{
  for (int i=0; i<m_regCounter; ++i) {
			if (m_list[i].commandlength == serialcmdpoint) {
				if ( compareStr(m_list[i].command, serialcmd, serialcmdpoint) ) {
					(*m_list[i].f)();
					return;
				}
			}
  }
}


bool Zcommand::compareStr(char* x, char* y, int l)
{
	for(int i=0; i<l; ++i) 
		if (x[i] != y[i]) return false;
	return true;
}