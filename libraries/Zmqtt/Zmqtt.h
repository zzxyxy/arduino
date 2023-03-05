#include <Client.h>
#include <PubSubClient.h>

#define ZDEBUG

class Zmqtt {
    PubSubClient* mqttClient;

    public:
        Zmqtt(Client& client, char* server, char* mqttuser, char* mqttpass);

        void loop();
        void subscribeReceive(char* topic, byte* payload, unsigned int length);
        void callback(MQTT_CALLBACK_SIGNATURE);
        void subscribeTopic(char* topic);
};
