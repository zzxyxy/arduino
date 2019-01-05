#include <SoftwareSerial.h>
#include "Zchaos.h"

#ifndef ZWIFI_H
#define ZWIFI_H

class Zwifi 
{
public:
    Zwifi(int rx, int tx, bool debug = false);
    void setup();
    void loop();
    void sendToSerial(char c);
    void setConn(int id, char* ip, int port, char* proto);
    void send(int id, char* msg,  int len);

private:

    SoftwareSerial serial;
    int wifiState = WIFI_CONFIG;
    int charBufferPos=0;
    char charBuffer[100];
    bool logging;
    char* _ip[4];
    int _port[4];
    char* _proto[4];
    bool tmp = true;

    void readWifiSerial();
    void processWifiMsg(char* msg, int len);
    void log(char* txt);

static bool compareChars(char* char1, int len1, char* char2, int len2);
    
};


#endif
