#include "Zmqtt.h"


Zmqtt::Zmqtt(Client& client, char* server, char* mqttuser, char* mqttpass) {
  mqttClient = new PubSubClient(client);
  mqttClient->setServer(server, 1883);

  long randNumber = random(10000);
  char name[10];

  sprintf(name, "Noah-", randNumber);
  if (mqttClient->connect(name, mqttuser, mqttpass))
  {
#ifdef ZDEBUG
    Serial.println("Connection has been established, well done");
#endif

  }
#ifdef ZDEBUG
  else
  {
    Serial.println("Looks like the server connection failed...");
  }
#endif
}


void Zmqtt::loop() {
    mqttClient->loop();
}


void Zmqtt::subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  // Print the topic
  Serial.print("Topic: ");
  Serial.println(topic);

  // Print the message
  Serial.print("Message: ");
  for(int i = 0; i < length; i ++)
  {
    Serial.print(char(payload[i]));
  }

  // Print a newline
  Serial.println("");
}


void Zmqtt::callback(MQTT_CALLBACK_SIGNATURE) {
    mqttClient->setCallback(callback);
}

void Zmqtt::subscribeTopic(char* topic) {
    mqttClient->subscribe(topic);
}

bool Zmqtt::publish(const char* topic, const char* payload) {
  return mqttClient->publish(topic, payload);
}
