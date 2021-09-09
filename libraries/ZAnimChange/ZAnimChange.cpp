#include "ZAnimChange.h"
#include "Arduino.h"

ZAnimChange::ZAnimChange(int from, int to, changeF* f) 
{
    m_from = from;
    m_to = to;
    m_f = f
    m_current = m_from;

    int distace = m_from - m_to;
    m_step = (distance / (ZANIMTIME / ZANIMWAIT));
}


ZAnimChange ZAnimChange::getInstance(int from, int to, changeF f)
{

}

void ZAnimChange::setup()
{
    m_nextTime = 0;
    for (int i=0; i< ZANIMINSTANCES; ++i) {
        instances[i] = nullptr;
    }
}


void ZAnimChange::loop()
{
    int time = millis();   
    if (time < m_nextTime) return;
    m_nextTime = time + m_wait;

    for (int i=0; i< ZANIMINSTANCES; ++i) {
        if (instances[i] != nullptr) {
            instances[i]->instanceLoop();
        }
    }

}


void ZAnimChange::instanceLoop()
{
    bool end = false;
    m_current = m_current - m_step;
    if (m_from > m_to) {
        if (m_current < m_to) {
            m_current = m_to;
            end = true;
        }
    } else {
        if (m_current > m_to) {
            m_current = m_to;
            end = true;
        }
    }

    &m_f(m_current);
}
