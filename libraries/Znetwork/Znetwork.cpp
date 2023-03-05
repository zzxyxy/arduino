#include "Znetwork.h"

Znetwork::Znetwork(byte* ethmac) {
    mac = ethmac;
}

void Znetwork::setup() {
  Ethernet.init(10);
#ifdef ZDEBUG
  Serial.println("Initialize Ethernet with DHCP:");
#endif
  if (Ethernet.begin(mac) == 0) {
#ifdef ZDEBUG
    Serial.println("Failed to configure Ethernet using DHCP");
#endif
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
#ifdef ZDEBUG
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
#endif
    } else if (Ethernet.linkStatus() == LinkOFF) {
#ifdef ZDEBUG
      Serial.println("Ethernet cable is not connected.");
#endif
    }
    // no point in carrying on, so do nothing forevermore:
  }
  // print your local IP address:
#ifdef ZDEBUG
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
#endif
}


void Znetwork::loop() {
#ifdef ZDEBUG
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    default:
      //nothing happened
      break;
  }
#endif
}

EthernetClient* Znetwork::getEthernetClient() {
    return &ethClient;
}
