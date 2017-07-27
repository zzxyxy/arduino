#include "Zwifi.h"

// AT+IPR=9600


Zwifi wifi(9 ,10, true);  

char tmpip[] = "172.20.0.2\0"; 
char tmpprot[] = "UDP\0"; 

void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("Boot!");

//AT+CWMODE=1

  Serial.println("Boot done!");
  wifi.setup();
  wifi.setConn(1, tmpip, 514, tmpprot); 
}

void loop() 
{
  if (Serial.available()) {
    char x = Serial.read();
    wifi.sendToSerial(x);
  }
  wifi.loop();

}
