#include "Zwifi.h"
#include "Arduino.h"

Zwifi::Zwifi(int rx, int tx, bool debug):serial(rx, tx), logging(debug) 
{
  serial.begin(9600);
//  serial.write("AT+RST\r\n");
}


void Zwifi::setup()
{
    Serial.println("Start wifi");
    serial.print("AT+RST\r\n");
}


void Zwifi::loop()
{
  if (serial.available()) {
    readWifiSerial();
  }
  processWifiMsg("_loop_", 0); 
}


bool Zwifi::compareChars(char* char1, int len1, char* char2, int len2)
{
    if (len1!=len2) return false;
    for (int i=1; i<len1; ++i) 
        if (char1[i] != char2[i]) return false;
    return true;
}


void Zwifi::processWifiMsg(char* msg, int len)
{
  switch(wifiState) {
  case WIFI_DISCONNECTED:
      if (0==len) {
        serial.print("AT+CWJAP=\"zxyxy\",\"kaiserschmarrn\"\r\n");
        wifiState=WIFI_CONNECTING;
        log("connecting");
      }
      break;
    case WIFI_CONNECTED: {
        if (tmp) {
          char msg[] = "started";
          send(1, msg, sizeof(msg)-1);
        }
      }
      break;
    case WIFI_CONNECTING:
      if (compareChars("OK", 2, msg, len)) {
        wifiState=WIFI_CONNECTED;
        log("connected");
      }
      break;    
    case WIFI_FAILED:

      break;
    case WIFI_CONFIG:
      if (compareChars("ready", 5, msg, len)) {
        wifiState=WIFI_CONFIG2;
        serial.write("AT+CWMODE=1\r\n");
      }
      break;
    case WIFI_CONFIG2:
      if (compareChars("OK", 2, msg, len)) {
        wifiState=WIFI_CONFIG3;
        serial.print("AT+CIPMUX=1\r\n");
      }
      break;
    case WIFI_CONFIG3:
      if (compareChars("OK", 2, msg, len)) {
        wifiState=WIFI_DISCONNECTED;
        log("Wifi configured as client");
      }
      break;
  }
}



void Zwifi::readWifiSerial()
{
  while(serial.available()) {
    charBuffer[charBufferPos]=serial.read();
    ++charBufferPos;

    if ('\n' ==  charBuffer[charBufferPos-1]) {
      processWifiMsg(charBuffer, charBufferPos-2);
      charBuffer[charBufferPos] = 0x0;
        log(charBuffer);
      charBufferPos=0;
    }
  }
};


void Zwifi::sendToSerial(char c) 
{
  serial.write(c);
}


void Zwifi::log(char* txt) {
  if (logging) {
    Serial.println(txt);
  }
}


void Zwifi::setConn(int id, char* ip, int port, char* proto)
{
  _ip[id] = ip;
  _port[id] = port;
  _proto[id] = proto;  
}


void Zwifi::send(int id, char* msg, int len) 
{
  bool wait = true;
  serial.print("AT+CIPSTART=");
  serial.print(id, DEC);
  serial.print(",\"");
  serial.print(_proto[id]);
  serial.print("\",\"");
  serial.print(_ip[id]);
  serial.print("\",");
  serial.print(_port[id]);
  serial.print("\r\n");
  tmp = false;

  while(wait)
  while(serial.available()) {
    charBuffer[charBufferPos]=serial.read();
    ++charBufferPos;
    if ('\n' ==  charBuffer[charBufferPos-1]) {
      if (compareChars("OK", 2, charBuffer, charBufferPos-2))
        wait=false;
      charBuffer[charBufferPos] = 0x0;
        log(charBuffer);
      charBufferPos=0;
    }
  }
  serial.print("AT+CIPSEND=");
  serial.print(id, DEC);
  serial.print(",");
  serial.print(len);
  serial.print("\r\n");
  wait = true;
  while(wait)
  while(serial.available()) {
    charBuffer[charBufferPos]=serial.read();
    ++charBufferPos;
    if ('\n' ==  charBuffer[charBufferPos-1]) {
      if (compareChars("OK", 2, charBuffer, charBufferPos-2))
        wait=false;
      charBuffer[charBufferPos] = 0x0;
        log(charBuffer);
      charBufferPos=0;
    }
  }

  serial.print(msg);
  serial.print("\r\n");
  wait = true;
  while(wait)
  while(serial.available()) {
    charBuffer[charBufferPos]=serial.read();
    ++charBufferPos;
    if ('\n' ==  charBuffer[charBufferPos-1]) {
      if (compareChars("SEND OK", 7, charBuffer, charBufferPos-2))
        wait=false;
      charBuffer[charBufferPos] = 0x0;
        log(charBuffer);
      charBufferPos=0;
    }
  }

  serial.print("AT+CIPCLOSE=");
  serial.print(id, DEC);
  serial.print("\r\n");
}


/*
 
AT+CIPSTART=1,"UDP","172.20.0.2",514

AT+CIPSEND=1,11

AT+CIPCLOSE=1


*/

