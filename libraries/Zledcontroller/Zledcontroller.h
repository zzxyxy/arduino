#ifndef ZLEDCONTROLLER
#define ZLEDCONTROLLER

class Zledcontroller 
{
public:
  enum State {
    ON,
    OFF
  };
  Zledcontroller(const int dimPin);
  Zledcontroller(const int dimPin, const int switchPin);
  void setup();
  void loop();
  
  void set(int dim);
  State getState() const;
private:
  const int mDimPin;
  const int mSwitchPin;
  bool mSwitch = false;
  
  unsigned long wait = 40;
  unsigned long timestamp = 0;
  unsigned long next = 0;  

  int actualDim = 0;
  int setDim = 0;
};


#endif // ZLEDCONTROLLER
