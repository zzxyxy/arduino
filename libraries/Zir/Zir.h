
#ifndef ZIR_H
#define ZIR_H


#include <IRremote.h>


typedef void (*irFunc)(bool);

class Zir {
public:
  Zir(int pin);
//  virtual ~Zir();
//  void loop();
//  void registerHandler(unsigned long code, void (*f)(bool), bool continguous = false);
//  void run(unsigned long code, bool again, bool _long = false);
//  void setup();
//
//private:
//  struct item {
//    unsigned long code;
//    irFunc f;
//    bool con;   // continguous
//  };
//  struct item m_list[30];
//  static const int m_infraWait = 200;
//  static const unsigned long m_reply = 4294967295;
//  unsigned long m_lastvalue = 0;
//  unsigned long m_regCounter = 0;
  unsigned long m_pin;
//  unsigned long m_IR_nextTime = 0;
//  bool m_again = false;
  IRrecv m_irrecv;
//  decode_results m_results;
//  unsigned long m_pressedtimestamp= 0;
};

#endif
