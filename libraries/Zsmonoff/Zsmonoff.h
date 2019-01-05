#ifndef ZDMONOFF_H
#define ZDMONOFF_H

#include <Arduino.h>

class Zsmonoff
{
public:
  enum State {
    ON,
    OFF
  };
  
  Zsmonoff(int pin);
  Zsmonoff(const Zsmonoff& orig);
  virtual ~Zsmonoff();
  void setup();
  void switching();
  void on();
  void off();
  State getState();
private:
  State m_state = OFF;
  const int m_pin;
};

#endif /* ZDMONOFF_H */

