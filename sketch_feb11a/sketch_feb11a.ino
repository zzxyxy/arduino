#include "Zserial.h"

Zserial zser;


void x(long x)
{
  Serial.print("xx : ");
  Serial.println(x, HEX);
}


void setup() {
  zser.setup();
  zser.registerCommand(0x61616161, x);
  pinMode(13, OUTPUT); 
}


void loop() {
  zser.loop();
}

/*
      switch(x) {
      case 0x61:
        digitalWrite(13, HIGH);
      break;
      case 0x62:
        digitalWrite(13, LOW);
      break;
    }

 */
