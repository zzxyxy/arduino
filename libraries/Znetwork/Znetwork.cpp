#include "Znetwork.h"

Znetwork::Znetwork(byte* ethmac) {
    mac = ethmac;
}

void Znetwork::setup() {
  Ethernet.init(10);
  connect();
}


void Znetwork::loop() {
  connect();

  if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      isConnected = 0;
      return;
  }

  if (!ethClient.connected()) {
    isConnected = 0;
    return;
  }

#ifdef ZDEBUG
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      for (int i=0; i<clientcounter; ++i) {
        clients[i]->netDisconnect();
      }
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
      for (int i=0; i<clientcounter; ++i) {
        clients[i]->netDisconnect();
      }
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      for (int i=0; i<clientcounter; ++i) {
        clients[i]->netRebind();
      }
      break;

    default:
      //nothing happened
      break;
  }
#endif
}

void Znetwork::connect() {
  if (isConnected) return;

#ifdef ZDEBUG
  Serial.println("Initialize Ethernet with DHCP:");
#endif
  if (Ethernet.begin(mac) == 0) {
#ifdef ZDEBUG
    Serial.println("Failed to configure Ethernet using DHCP");
    isConnected = 0;
    return;
#endif
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
#ifdef ZDEBUG
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      isConnected = 0;
      return;
#endif
    } else if (Ethernet.linkStatus() == LinkOFF) {
#ifdef ZDEBUG
      Serial.println("Ethernet cable is not connected.");
      isConnected = 0;
      return;
#endif
    }
    // no point in carrying on, so do nothing forevermore:
  } else {
    isConnected = 1;
    for (int i=0; i<clientcounter; ++i) {
      clients[i]->netConnect();
    }
  }
  // print your local IP address:
#ifdef ZDEBUG
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
#endif
}

void Znetwork::subscribe(ZnetworkListener* client) {
  clients[clientcounter] = client;
  ++clientcounter;
  client->netConnect();
}


EthernetClient* Znetwork::getEthernetClient() {
    return &ethClient;
}
