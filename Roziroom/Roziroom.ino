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
#include <ZirEsp.h>

#define CLIENTNAME "Roziroom"
#define TOPIC "RoziRoom"

EspMQTTClient client(
  "zxyxy_iot",
  "mPHhrbF3NZq2wXUJ5GrMAvSX7PMf8Mwc9eH",
  "172.20.1.1",  // MQTT Broker server ip
  CLIENTNAME      // Client name that uniquely identify your device
);

ZirEsp ir(4); 

#define NUM_LEDS 118     //118
#define ALL_LEDS 134     //118
#define DATA_PIN 1
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[ALL_LEDS];

//#define SCAN_TIME 30     // seconds

// IRrecv irrecv(4, 1024, 50, true);
// decode_results results;

int gr = 0;
int gg = 0;
int gb = 0;


int reader_on = CRGB::Black;
void reader(bool x)
{
  if (reader_on == CRGB::Black) {
    reader_on = CRGB::White;
  } else {
    reader_on = CRGB::Black;    
  }
  
  for (unsigned i = NUM_LEDS+1 ; i < ALL_LEDS; ++i) {
    leds[i] = reader_on;
  }

  FastLED.show();  
}

void setAllLed(int color)
{

  for (unsigned i = 0; i < NUM_LEDS; ++i) {
    leds[i] = color;  //CRGB::Black;
  }


  FastLED.show();  
}


void resetLeds() 
{
  setAllLed(CRGB::Black);
}


void setAllWhite(int bright) 
{
  gr = bright;
  gb = bright;
  gg = bright;
  StaticJsonDocument<100> doc;
  JsonObject obj = doc.to<JsonObject>();  
  
  int color = bright;
  color = color + bright * 0x100;
  color = color + bright * 0x10000;
  setAllLed(color);

  obj["lamp"] = 1;
  obj["w"] = bright;
  String str; 
  serializeJson(doc, str);

  client.publish(TOPIC "_ans", str);
}


void setAllColor(int r, int g, int b) 
{
  StaticJsonDocument<100> doc;
  JsonObject obj = doc.to<JsonObject>();  
  
  int color = b;
  color = color + r * 0x100;
  color = color + g * 0x10000;
  setAllLed(color);

  obj["lamp"] = 1;
  obj["r"] = r;
  obj["g"] = g;
  obj["b"] = b;
  String str; 
  serializeJson(doc, str);

  gr = r;
  gb = g;
  gg = b;

  client.publish(TOPIC "_ans", str);
}


void setBrighter(int val) 
{
  int r = gr + val;
  int g = gg + val;
  int b = gb + val;
  if (r > 255) r = 255;
  if (g > 255) g = 255;
  if (b > 255) b = 255;
  setAllColor(r, g, b);  
}


void setDarker(int val) 
{
  int r = gr - val;
  int g = gg - val;
  int b = gb - val;
  if (r < 0) r = 0;
  if (g < 0) g = 0;
  if (b < 0) b = 0;
  setAllColor(r, g, b);  
}

void switchOnOff(bool x) 
{
  if (gg == 0 && gb == 0 && gr ==0) {
    setAllWhite(255);
  } else {
    setAllWhite(0);    
  }
}


int getSmallest()
{
  if (gr < gg && gr < gb) return gr;
  if (gg < gr && gg < gb) return gg;
  return gb;
}


int getDimVal(int x) 
{
  if (x<5) return 1;
  if (x<10) return 3;
  if (x<15) return 5;
  if (x<50) return 10;
  if (x<100) return 15;
  if (x<170) return 20;
  return 30;
}


void dim_up(bool x) 
{
  setBrighter(getDimVal(getSmallest()));
}


void dim_down(bool x) 
{
  setDarker(getDimVal(getSmallest()));
}


void night(bool x) {setAllColor(8, 8, 2);}
void lila(bool x) {setAllColor(8, 0, 8);}

int colorCounter = 0;
void colorSwitch(bool x) 
{
  switch (colorCounter) {
    case 0:
      setAllColor(255, 20, 147);
      break;
    case 1:
      setAllColor(255, 0, 0);
      break;
    case 2:
      setAllColor(105, 12, 12);
      break;
    case 3:
      setAllColor(255, 69, 0);
      break;
    case 4:
      setAllColor(255, 215, 0);
      break;
    case 5:
      setAllColor(255, 255, 0);
      break;
    case 6:
      setAllColor(124, 252, 0);
      break;
    case 7:
      setAllColor(0, 255, 255);
      break;
    case 8:
      setAllColor(0, 191, 255);
      break;
    case 9:
      setAllColor(75, 0, 130);
      colorCounter = -1;
      break;
  }

  ++colorCounter;
}

void setup() 
{
  delay(1000);


  FastLED.addLeds<WS2812, DATA_PIN>(leds, ALL_LEDS);
  resetLeds();
  FastLED.show();

  Serial.begin(9600);

  ir.setup();
  ir.registerHandler(IR_REMOTE_CODE_POWER, switchOnOff, false);
  ir.registerHandler(IR_REMOTE_CODE_VOL_UP, dim_up, true);
  ir.registerHandler(IR_REMOTE_CODE_VOL_DOWN, dim_down, true);
  ir.registerHandler(IR_REMOTE_CODE_1, night, false);
  ir.registerHandler(IR_REMOTE_CODE_2, lila, false);
  ir.registerHandler(IR_REMOTE_CODE_7, colorSwitch, false);
  ir.registerHandler(IR_REMOTE_CODE_9, reader, false);
}


void onConnectionEstablished() {
  client.subscribe(TOPIC, [] (const String &payload) {
    StaticJsonDocument<100> doc;
    StaticJsonDocument<100> colors;
    deserializeJson(doc, payload);

    String w = doc["w"];
    String r = doc["r"];
    String g = doc["g"];
    String b = doc["b"];
    String bright = doc["bright"];

    if (w != "null") {
//      Serial.println(w);
      setAllWhite(w.toInt());
      return;
    }

    if (r != "null" && b != "null" && g != "null") {
      setAllColor(r.toInt(), g.toInt(), b.toInt());
      return;
    }

    if (bright != "null") {
      if (bright == "+") setBrighter(15);
      if (bright == "-") setDarker(15);     
      return;
    }
  });
}


void loop() {
  client.loop();
  ir.loop();
}
