#include <Arduino.h>

typedef void (*ZserFunc)(long);

#define WAIT 10000

class Zserial 
{
public:
  void setup();
  void loop();

  void registerCommand(long code, void (*f)(long));

private:
  long comm=0;
  long parameter=0;
  bool param=false;
  long shift = 0x1000000;

  unsigned long timestamp = 0;
  bool progress = false;

  struct item {
    long code;
    ZserFunc f;
  };

  struct item m_list[50];
  int m_regCounter = 0;

  void run(long command, long parameter);
  void reset();
};
