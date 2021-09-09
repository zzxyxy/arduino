#include <i18n.h>
#include <IRac.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include "EspMQTTClient.h"
#include "ArduinoJson.h"
#include "FastLED.h"
#include "Zchaos.h"

EspMQTTClient client(
  "zxyxy_iot",
  "mPHhrbF3NZq2wXUJ5GrMAvSX7PMf8Mwc9eH",
  "172.20.1.1",  // MQTT Broker server ip
  "RoziRoom"      // Client name that uniquely identify your device
);


#define NUM_LEDS 150
#define DATA_PIN 1
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

//#define SCAN_TIME 30     // seconds

// int pin = 4;  // d2

IRrecv irrecv(4, 1024, 50, true);
decode_results results;

void setup() 
{
  delay(1000);
  FastLED.addLeds<WS2812, DATA_PIN>(leds, NUM_LEDS);

  for (unsigned i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::White;
  }
  
  Serial.begin(9600);
  /*
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;
  leds[NUM_LEDS - 3] = CRGB::Blue;
  leds[NUM_LEDS - 2] = CRGB::Red;  */

  FastLED.show();

  irrecv.setUnknownThreshold(12);
  irrecv.enableIRIn();  // Start the receiver
}


void onConnectionEstablished() {
//  client.subscribe("workroom", [] (const String &payload)  {
//    StaticJsonDocument<256> doc;
//    deserializeJson(doc, payload);

//    String x = doc["lamp1"];
//   analogWrite(pin, x.toInt());
//    Serial.println(x);
//  });
}

int led = 1;
void ledswitch()
{
  if (led)
  {
    led = 0;
  } else {
    led = 1;
  }
}


void loop() {
  client.loop();
  // Now turn the LED off, then pause
  //delay(2000);
  //digitalWrite(pin, HIGH);       
  //delay(2000);
  //digitalWrite(pin, LOW);       

  if (irrecv.decode(&results)) {
    Serial.println((unsigned) results.value, DEC);
//    Serial.print(resultToHumanReadableBasic(&results));
  }

//  StaticJsonDocument<100> res;
//  serializeJson(res, out);
//  client.publish("test", "test");
}
