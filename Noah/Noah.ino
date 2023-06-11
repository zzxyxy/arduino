#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Znetwork.h>
#include <Zmqtt.h>
#include "FastLED.h"
#include "ZFastLed.h"
#include "Zledcontroller.h"

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


#define NUM_LEDS 17
#define DATA_PIN 25
#define DATA_PIN2 29

ZFastLed led1;
ZFastLed led2;
Zledcontroller led3(44);
Zledcontroller led4(46);

void setup() {

//   pinMode(PANELLED, HIGH);
#ifdef ZDEBUG
   Serial.begin(9600);
   while (!Serial) {}
   Serial.println("Start controller");
#endif
  led3.setup();
  led4.setup();
  led3.set(0);
  led4.set(0);
  net = new Znetwork(mac);  
  net->setup();
  delay(3000);
  m = new Zmqtt(*net->getEthernetClient(), server, mqttuser, mqttpass);  
  m->callback(subscribeReceive);
  m->subscribeTopic("core");
  net->subscribe(m);

  led1.setup<WS2812, DATA_PIN>(17, 9);
  led2.setup<WS2812, DATA_PIN2>(17, 9);
  delay(300);
  led1.setAllColor(0x000000);
  led2.setAllColor(0x000000);
}

void loop() {
  net->loop();
  m->loop(); 
  led3.loop();
  led4.loop();

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

#define REPLYSIZE 200
  char reply[REPLYSIZE];


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
      m->publish(doc["topic"], "{\"messagetype\": \"ping\"}");
    }
    if (req == "lamp1") {
      Serial.println("Lamp1");
      String r = doc["r"];
      String g = doc["g"];
      String b = doc["b"];
      led2.setAllColor(r.toInt(), g.toInt(), b.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp1\", \"r\": " + r + ", \"g\": " + g + ", \"b\": " + b + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish("core", reply);
    }
    if (req == "lamp2") {
      Serial.println("Lamp2");
      String r = doc["r"];
      String g = doc["g"];
      String b = doc["b"];
      led1.setAllColor(r.toInt(), g.toInt(), b.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp2\", \"r\": " + r + ", \"g\": " + g + ", \"b\": " + b + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish("core", reply);
    }
    if (req == "lamp3") {
      Serial.println("Lamp3");
      String w = doc["w"];
      led3.set(w.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp3\", \"w\": " + w + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish("core", reply);
    }
    if (req == "lamp4") {
      Serial.println("Lamp4");
      String w = doc["w"];
      led4.set(w.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp4\", \"w\": " + w + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish("core", reply);
    }
  }

  // Print the message
  Serial.print("Message: ");
  for(int i = 0; i < length; i ++)
  {
    Serial.print(char(payload[i]));
  }
}


 