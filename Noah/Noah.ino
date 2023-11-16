#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Znetwork.h>
#include <Zmqtt.h>
#include "FastLED.h"
#include "ZFastLed.h"
#include "Zledcontroller.h"
#include "DHT.h"

#define MQTTNAME "Noah"
#define ZDEBUG
#define PANELLED 13
const char* server = "zxyxyhome.duckdns.org";
const char* mqttuser = "iot";
const char* mqttpass = "RyzYK8G53ZCnOR1OKlYL28yNoOagKG";
const char* mgttname = MQTTNAME;
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
#define MQTTNAME "noah"
#define DHT22_PIN 4

ZFastLed led1;
ZFastLed led2;
Zledcontroller led3(44);
Zledcontroller led4(46);
DHT temperature(DHT22_PIN, DHT11);

#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library

void setup() {

//   pinMode(PANELLED, HIGH);
#ifdef ZDEBUG
   Serial.begin(9600);
   while (!Serial) {}
   Serial.println("Start controller");
#endif
  const char* mqttname = MQTTNAME;

  led3.setup();
  led4.setup();
  led3.set(0);
  led4.set(0);
  net = new Znetwork(mac);  
  net->setup();
  delay(300);
  m = new Zmqtt(*net->getEthernetClient(), server, mqttuser, mqttpass, mqttname);  
  m->callback(subscribeReceive);
  m->subscribeTopic(LAMPTOPIC);
  net->subscribe(m);

  led1.setup<WS2812, DATA_PIN>(17, 9);
  led2.setup<WS2812, DATA_PIN2>(17, 9);
  delay(300);
  led1.setAllColor(0x000000);
  led2.setAllColor(0x000000);
  irsetup();

  temperature.begin();
}

void loop() {
  irloop();
  temploop();
  net->loop();
  m->loop(); 
  led3.loop();
  led4.loop();
}

unsigned long lasttime=0;

void temploop() {
  unsigned long mytime = millis();  
  if (mytime < lasttime + 10000) {
    return;
  }
  lasttime = mytime;

  // read humidity
  float humi  = temperature.readHumidity();
  // read temperature as Celsius
  float tempC = temperature.readTemperature();
  // read temperature as Fahrenheit
  float tempF = temperature.readTemperature(true);

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    Serial.print("DHT22# Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
  }
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
    else if (req == "lamp1") {
      String r = doc["r"];
      String g = doc["g"];
      String b = doc["b"];
      led2.setAllColor(r.toInt(), g.toInt(), b.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp1\", \"r\": " + r + ", \"g\": " + g + ", \"b\": " + b + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    else if (req == "lamp2") {
      String r = doc["r"];
      String g = doc["g"];
      String b = doc["b"];
      led1.setAllColor(r.toInt(), g.toInt(), b.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp2\", \"r\": " + r + ", \"g\": " + g + ", \"b\": " + b + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    else if (req == "lamp3") {
      String w = doc["w"];
      led3.set(w.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp3\", \"w\": " + w + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    else if (req == "lamp4") {
      String w = doc["w"];
      led4.set(w.toInt());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp4\", \"w\": " + w + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    else if (req == "getlamp1") {
      CRGB lamp = led2.getLed(0);
      String r = String(lamp.g);
      String g = String(lamp.r);
      String b = String(lamp.b);
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp1\", \"r\": " + r + ", \"g\": " + g + ", \"b\": " + b + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    else if (req == "getlamp2") {
      CRGB lamp = led1.getLed(0);
      String r = String(lamp.g);
      String g = String(lamp.r);
      String b = String(lamp.b);
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp2\", \"r\": " + r + ", \"g\": " + g + ", \"b\": " + b + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    else if (req == "getlamp3") {
      String w = String(led3.get());
      String replys = "{\"messagetype\": \"lampstatus\", \"name\": \"lamp3\", \"w\": " + w + "}";
      replys.toCharArray(reply, REPLYSIZE);
      m->publish(LAMPTOPIC, reply);
    }
    else if (req == "getlamp4") {
      String w = String(led4.get());
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