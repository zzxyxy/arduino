#include <Arduino.h>
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

#define REPLYSIZE 200
#define NUM_LEDS 17
#define DATA_PIN 25
#define DATA_PIN2 29
#define LAMPTOPIC "lamp"
#define IRTOPIC "irremote"

ZFastLed led1;
ZFastLed led2;
Zledcontroller led3(44);
Zledcontroller led4(46);

#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

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
  delay(300);
  m = new Zmqtt(*net->getEthernetClient(), server, mqttuser, mqttpass);  
  m->callback(subscribeReceive);
  m->subscribeTopic(LAMPTOPIC);
  net->subscribe(m);

  led1.setup<WS2812, DATA_PIN>(17, 9);
  led2.setup<WS2812, DATA_PIN2>(17, 9);
  delay(300);
  led1.setAllColor(0x000000);
  led2.setAllColor(0x000000);
  irsetup();
  Serial.println("Ready");
}

void loop() {
  irloop();
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

  char reply[REPLYSIZE];

  if (err) return;    

  Serial.println("");
  Serial.println("----------------------");
  String req = doc["req"]; 
  Serial.println(req);
  // Print a newline
  Serial.println("");

  if (topic == LAMPTOPIC) {
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
      m->publish(LAMPTOPIC, reply);
    }
    if (req == "lamp2") {
      Serial.println("Lamp2");
      String r = doc["r"];
      String g = doc["g"];
      String b = doc["b"];
      led1.setAllColor(r.toInt(), g.toInt(), b.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp2\", \"r\": " + r + ", \"g\": " + g + ", \"b\": " + b + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    if (req == "lamp3") {
      Serial.println("Lamp3");
      String w = doc["w"];
      led3.set(w.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp3\", \"w\": " + w + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    if (req == "lamp4") {
      Serial.println("Lamp4");
      String w = doc["w"];
      led4.set(w.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp4\", \"w\": " + w + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
  }

  // Print the message
  Serial.print("Message: ");
  for(int i = 0; i < length; i ++)
  {
    Serial.print(char(payload[i]));
  }
}

void irsetup() {
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));
}
 
void irloop() {
    if (IrReceiver.decode()) {
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.printIRSendUsage(&Serial);
        IrReceiver.resume(); 
        char reply[REPLYSIZE];
        String replys = "{\"messagetype\": \"received\", \"receiver\": \"noah\", \"command\": " + String(IrReceiver.decodedIRData.command) + 
        ", \"decodedRawData\": " + String(IrReceiver.decodedIRData.decodedRawData) + 
        ", \"decodedRawData\": " + String(IrReceiver.decodedIRData.decodedRawData) +
        ", \"protocol\": " + String(IrReceiver.decodedIRData.protocol) +
        ", \"extra\": " + String(IrReceiver.decodedIRData.extra) +
        ", \"flags\": " + String(IrReceiver.decodedIRData.flags) +        
        " }";
        replys.toCharArray(reply, IRTOPIC);
        m->publish(IRTOPIC, reply);
    }
} 