#include "Zir.h"

Zir::Zir(int pin):m_pin(pin),m_irrecv(m_pin)
{

}


Zir::~Zir() 
{
//  Serial.println("class deleted");
}


void Zir::loop() 
{
	m_again = false;
  if (m_IR_nextTime > (millis() + 10000)) m_IR_nextTime = millis() + m_infraWait;
  if (millis() < m_IR_nextTime) return;
  m_IR_nextTime = millis() + m_infraWait;
	
  if (m_irrecv.decode(&m_results)) {
		if (m_reply == m_results.value) {
			m_again = true;
  		m_results.value = m_lastvalue;
		} else {
			m_pressedtimestamp = millis();
		}
#ifdef ZDEBUG
    Serial.print("IR recived on pin (");
    Serial.print(m_pin);
    Serial.print(") :"); 
    Serial.println(m_results.value);
#endif
    m_lastvalue = m_results.value;
    m_irrecv.resume(); // Receive the next value
  } else {
    m_lastvalue = 0;
  }
		if (0 !=  m_lastvalue) {
			if (m_pressedtimestamp + 2000 > millis()) {
				run(m_results.value, m_again, false);

			} else {
				run(m_results.value, m_again, true);
			}			
		
	  }

}


void Zir::setup()
{
	m_irrecv.enableIRIn();	
}


void Zir::run(unsigned long code, bool again, bool _long) 
{
  for (int i=0; i<m_regCounter; ++i) {
    if (m_list[i].code == code) {
			if (again && ! m_list[i].con) return;
      m_list[i].f(_long);
      return;
    }
  }
}


void Zir::registerHandler(unsigned long code, void (*f)(bool), bool continguous) 
{
  m_list[m_regCounter].code = code;
  m_list[m_regCounter].f = &(*f);
  m_list[m_regCounter].con = continguous;
  ++m_regCounter;
}
