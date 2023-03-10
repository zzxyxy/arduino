#include "Zsmonoff.h"


Zsmonoff::Zsmonoff(int pin):m_pin(pin)
{
}


Zsmonoff::Zsmonoff(const Zsmonoff& orig)
{
}


Zsmonoff::~Zsmonoff()
{
}


Zsmonoff::State Zsmonoff::getState()
{
  return m_state;
}


void Zsmonoff::setup() {
	pinMode(m_pin, OUTPUT);
	digitalWrite(m_pin, HIGH);     
}


void Zsmonoff::switching() {
//	Serial.print("switch ");
//	Serial.print(m_pin);
  if(ON == m_state) {
    m_state=OFF;
    digitalWrite(m_pin,HIGH);    
//		Serial.print(" OFF");
  } else {
    m_state=ON;
    digitalWrite(m_pin,LOW);    
//		Serial.print(" ON");
  }
//	Serial.println();
}


void Zsmonoff::on() {
	m_state=ON;
	digitalWrite(m_pin,LOW);    
}


void Zsmonoff::off() {
	m_state=OFF;
	digitalWrite(m_pin,HIGH);    
}