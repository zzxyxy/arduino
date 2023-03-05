#include <SPI.h>
#include <Ethernet.h>

#define ZDEBUG

class Znetwork {
    EthernetClient ethClient;
    byte* mac;

    public:
        Znetwork(byte* ethmac);
        void setup();
        void loop();
        EthernetClient* getEthernetClient();
};
