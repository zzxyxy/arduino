#include "ZirEsp.h"
#include "Arduino.h"

ZirEsp::ZirEsp(int pin):m_pin(pin),m_irrecv(m_pin, 1024, 50, true)
{

}


ZirEsp::~ZirEsp() 
{
//  Serial.println("class deleted");
}


void ZirEsp::loop() 
{
	m_again = false;
  if (m_IR_nextTime > (millis() + 5000)) m_IR_nextTime = millis() + m_infraWait;
  if (millis() < m_IR_nextTime) return;
  m_IR_nextTime = millis() + m_infraWait;
	
  if (m_irrecv.decode(&m_results)) {
		if (m_reply == (unsigned long) m_results.value) {
			m_again = true;
		} else {
      m_lastvalue = m_results.value;
		}
#ifdef ZDEBUG
    Serial.print("IR recived on pin (");
    Serial.print(m_pin, DEC);
    Serial.print(") :"); 
    Serial.println((unsigned) m_results.value, DEC);
#endif
    m_irrecv.resume(); // Receive the next value
    run(m_lastvalue, m_again, false);
  }

}


void ZirEsp::setup()
{
	m_irrecv.enableIRIn();	
}


void ZirEsp::run(unsigned long code, bool again, bool _long) 
{
//  Serial.println(code, DEC);
  for (int i=0; i<m_regCounter; ++i) {
    if (m_list[i].code == code) {
			if (again && ! m_list[i].con) return;
      m_list[i].f(_long);
      return;
    }
  }
}


void ZirEsp::registerHandler(unsigned long code, void (*f)(bool), bool continguous) 
{
  m_list[m_regCounter].code = code;
  m_list[m_regCounter].f = &(*f);
  m_list[m_regCounter].con = continguous;
  ++m_regCounter;
}
