#include <SPI.h>
#include <Ethernet.h>

#define ZDEBUG
#ifndef ZNETWORK_H
#define ZNETWORK_H

class ZnetworkListener {
public:
    ZnetworkListener(){};
    virtual void netConnect() = 0;
    virtual void netRebind() = 0;
};

class Znetwork {
    EthernetClient ethClient;
    byte* mac;
    ZnetworkListener* clients[10];
    int clientcounter = 0;

    public:
        Znetwork(byte* ethmac);
        void setup();
        void loop();
        EthernetClient* getEthernetClient();
        void subscribe(ZnetworkListener* client);
};

#endif
