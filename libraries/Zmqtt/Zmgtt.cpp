#include "Zmqtt.h"


Zmqtt::Zmqtt(Client& client, char* server, char* mqttuser, char* mqttpass) {
  mqttClient = new PubSubClient(client);
  mqttClient->setServer(server, 1883);
  user = mqttuser;
  pass = mqttpass;
}


void Zmqtt::connect() {
  if (isConnected) return;
  long randNumber = random(10000);
  char name[10];

  sprintf(name, "Noah-", randNumber);
  if (mqttClient->connect(name, user, pass))
  {
#ifdef ZDEBUG
    Serial.println("MQTT connected");
#endif
    isConnected = 1;
  }
#ifdef ZDEBUG
  else
  {
    Serial.println("MQTT connection failed");
  }
#endif

mqttClient->subscribe(topic);
}


void Zmqtt::loop() {
  if (!mqttClient->connected()) {
    disConnect();
  }
  mqttClient->loop();
  if (!isConnected) {
    connect();
  }
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

void Zmqtt::disConnect() {
  isConnected = 0;
  mqttClient->disconnect();
}

void Zmqtt::callback(MQTT_CALLBACK_SIGNATURE) {
    mqttClient->setCallback(callback);
}

void Zmqtt::subscribeTopic(char* topic) {
    this->topic = topic;
}

bool Zmqtt::publish(const char* topic, const char* payload) {
  return mqttClient->publish(topic, payload);
}

void Zmqtt::netConnect() {
  connect();
}

void Zmqtt::netRebind() {
  connect();
}

void Zmqtt::netDisconnect() {
  disConnect();
}
