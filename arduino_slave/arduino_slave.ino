#include <Zir.h>

int incomingByte = 0;
Zir ir(4);

#define SERIAL_INPUT_LENGTH 200

char serialInput[SERIAL_INPUT_LENGTH];
int serialPos=0;
long timeCounter=0;

void resetSerialInput() 
{
  for (int i=0; i<SERIAL_INPUT_LENGTH; ++i) 
    serialInput[i] = 0;
  serialPos=0;
}

bool eqChar(char* a, int la, char* b, int lb ) {
  if (la != lb) return false;
  for (int i=0; i<la; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}


void setup() {
  resetSerialInput();
  ir.setup();
//  ir.registerHandler(16753245, testf, false);
  Serial.begin(9600);
  delay(2);
  Serial.println("Ready.");
}


void loop() {
  ++timeCounter;
  ir.loop();
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if ( 0xA == incomingByte ) {
//      Serial.println(serialInput);

      if ( eqChar("ping", 4, serialInput, serialPos) )
        Serial.println("pong");
      if ( eqChar("whoareyou", 4, serialInput, serialPos) )
        Serial.println("arduino");

      resetSerialInput();
    } else {
      serialInput[serialPos] = incomingByte;
      ++serialPos;
    }

  }

}

