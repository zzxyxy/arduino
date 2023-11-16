#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Znetwork.h>
#include <Zmqtt.h>
#include <Zsmonoff.h>

#define ZDEBUG
#define PANELLED 13
#define HEATSWITCHPIN 3


const char* server = "zxyxyhome.duckdns.org";
const char* mqttuser = "iot";
const char* mqttpass = "RyzYK8G53ZCnOR1OKlYL28yNoOagKG";

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xEE, 0xEE
};

Zmqtt* m;
Znetwork* net;
Zsmonoff* heat;

#define REPLYSIZE 200
#define NUM_LEDS 17
#define DATA_PIN 25
#define DATA_PIN2 29
#define LAMPTOPIC "lamp"
#define IRTOPIC "irremote"
#define MQTTNAME "Rosa"

#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Start controller");
  const char* mgttname = MQTTNAME;

  net = new Znetwork(mac);  
  net->setup();
  m = new Zmqtt(*net->getEthernetClient(), server, mqttuser, mqttpass, mgttname); 
  m->callback(subscribeReceive);
  m->subscribeTopic(LAMPTOPIC);
  net->subscribe(m);

  irsetup();

  heat = new Zsmonoff(HEATSWITCHPIN);
  heat->setup();
  heat->on();

  delay(1000);
}

void loop() {
  net->loop();
  m->loop(); 
  irloop();
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
        String replys = "{\"messagetype\": \"received\", \"receiver\": \"Rosa\", \"command\": " + String(IrReceiver.decodedIRData.command) + 
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
