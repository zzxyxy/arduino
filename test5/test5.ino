
#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(9, 10); // RX, TX




void setup()
{

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Boot!");


  wifiSerial.begin(9600);

  delay(800);



  
  Serial.println("Boot done!");

}

void loop() 
{
  if (wifiSerial.available()) {
    Serial.write(wifiSerial.read());
  }
  if (Serial.available()) {
     wifiSerial.write(Serial.read());
  }
}
