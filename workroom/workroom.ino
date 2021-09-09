#include "EspMQTTClient.h"
#include "ArduinoJson.h"
#include "FastLED.h"

EspMQTTClient client(
  "zxyxy_iot",
  "mPHhrbF3NZq2wXUJ5GrMAvSX7PMf8Mwc9eH",
  "172.20.2.1",  // MQTT Broker server ip
  "WorkRoom"      // Client name that uniquely identify your device
);

#define LISTEN_TOPIC "WorkRoom"
#define NOTIFY_TOPIC "WorkRoom_ans"
#define LAMP1 4

#define NUM_LEDS 17
#define DATA_PIN 4
int pin = 4;

CRGB leds[NUM_LEDS];

void setup() 
{
  delay(100);
  Serial.begin(9600);
  pinMode(LAMP1, OUTPUT);  
  digitalWrite(LAMP1, LOW);       
  
  FastLED.addLeds<WS2812, DATA_PIN>(leds, NUM_LEDS);
  delay(100);
  resetLeds();
}


void setAllLed(int color)
{
  Serial.println(color, HEX);
  for (unsigned i = 0; i < NUM_LEDS; ++i) {
    leds[i] = color;  //CRGB::Black;
  }
  FastLED.show();  
}


void setAllColor(int r, int g, int b) 
{
  int color = b;
  color = color + r * 0x100;
  color = color + g * 0x10000;
  setAllLed(color);
}


void resetLeds() 
{
  setAllLed(CRGB::Black);
}


void onConnectionEstablished() 
{
  client.subscribe(LISTEN_TOPIC, [] (const String &payload)  {
    StaticJsonDocument<100> doc;
    deserializeJson(doc, payload);

    
    String lampNum = doc["lamp"];
    String out;
    if (lampNum == "1") {
      String x = doc["w"];
      analogWrite(LAMP1, x.toInt());
  
      StaticJsonDocument<130> res;
      JsonObject obj = res.to<JsonObject>(); 
      obj["lamp"] = 1;
      obj["w"] = x.toInt();
      serializeJson(res, out);
      client.publish(NOTIFY_TOPIC, out);
    }

    if (lampNum == "2") {
      String r = doc["r"];
      String g = doc["g"];
      String b = doc["b"];
      
      setAllColor(r.toInt(), g.toInt(), b.toInt());
  
      StaticJsonDocument<130> res;
      JsonObject obj = res.to<JsonObject>(); 
      obj["lamp"] = 2;
      obj["r"] = r.toInt();
      obj["g"] = g.toInt();
      obj["b"] = b.toInt();
      serializeJson(res, out);
      client.publish(NOTIFY_TOPIC, out);
    }

  
  });
}


void loop() 
{
  client.loop();
}
