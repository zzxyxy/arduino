#include "EspMQTTClient.h"
#include "ArduinoJson.h"
#define LED 4


EspMQTTClient client(
  "zxyxy",
  "kaiserschmarrn",
  "172.20.1.1",  // MQTT Broker server ip
  "TestClient"      // Client name that uniquely identify your device
);


void setup() 
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}


void onConnectionEstablished() {
  String cw;

  client.subscribe("my", [] (const String &payload)  {
    Serial.println(payload);
  });

  StaticJsonDocument<200> doc;
  doc["title"] = "test title";
  doc["test"] = 42;
  serializeJson(doc, cw);

  client.publish("test", cw);
}

void loop() {
  client.loop();
}
