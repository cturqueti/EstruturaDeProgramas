#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h> // Biblioteca MQTT
#include <WiFi.h>         // Biblioteca Wi-Fi para ESP32
#include "secrets.h"

// Configurações do MQTT
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883

// Tópicos MQTT
#define MQTT_TOPIC_SUB "seu/topico/sub"
#define MQTT_TOPIC_PUB "seu/topico/pub"

class MQTTManager {
    public:
        MQTTManager(); 
        ~MQTTManager();
        void initMQTT();
        void handleMQTT();
        void publishMessage(const char* topic, const char* payload);
        void reconnectMQTT();

    private:
        WiFiClient _espClient;
        PubSubClient _mqttClient;
};

// Callback para mensagens recebidas
void mqttCallback(char* topic, byte* payload, unsigned int length);


#endif // MQTT_MANAGER_H