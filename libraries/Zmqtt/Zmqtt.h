#include <Client.h>
#include <Znetwork.h>
#include <PubSubClient.h>

#define ZDEBUG

class Zmqtt : public ZnetworkListener {
    PubSubClient* mqttClient;
    char* user;
    char* pass;
    char* topic;
    bool isConnected = 0;

    public:
        Zmqtt(Client& client, char* server, char* mqttuser, char* mqttpass);

        void loop();
        void connect();
        void disConnect();
        void subscribeReceive(char* topic, byte* payload, unsigned int length);
        void callback(MQTT_CALLBACK_SIGNATURE);
        void subscribeTopic(char* topic);
        bool publish(const char* topic, const char* payload);

        virtual void netConnect();
        virtual void netRebind();
        virtual void netDisconnect();
};
