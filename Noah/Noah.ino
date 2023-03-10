#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Znetwork.h>
#include <Zmqtt.h>

#define ZDEBUG
#define PANELLED 13
const char* server = "zxyxyhome.duckdns.org";
const char* mqttuser = "iot";
const char* mqttpass = "RyzYK8G53ZCnOR1OKlYL28yNoOagKG";

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

Zmqtt* m;
Znetwork* net;

void setupeth();
void setup_mqtt();
void subscribeReceive(char* topic, byte* payload, unsigned int length);

void setup() {
  pinMode(PANELLED, HIGH);
#ifdef ZDEBUG
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Start controller");
#endif

  net = new Znetwork(mac);  
  net->setup();
  delay(3000);
  m = new Zmqtt(*net->getEthernetClient(), server, mqttuser, mqttpass);  
  m->callback(subscribeReceive);
  m->subscribeTopic("core");
}


void loop() {
  net->loop();
  m->loop(); 
}

const size_t bufferSize = 128;
StaticJsonDocument<bufferSize> doc;

void subscribeReceive(char* topicchar, byte* payload, unsigned int length)
{

  String topic = topicchar;
  // Print the topic
  Serial.print("Topic: ");
  Serial.println(topic);

  DeserializationError err = deserializeJson(doc, payload);

  if (err) return;    

  Serial.println("");
  Serial.println("----------------------");
  String req = doc["req"]; 
  Serial.println(req);
  // Print a newline
  Serial.println("");

  if (topic == "core") {
    if (req == "ping") {
      Serial.println("I got a ping");
      m->publish(doc["topic"], "{\"ans\": \"ping\"}");
    }
  }

  // Print the message
  Serial.print("Message: ");
  for(int i = 0; i < length; i ++)
  {
    Serial.print(char(payload[i]));
  }
}


 