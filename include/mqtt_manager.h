#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h> // Biblioteca MQTT
#include <WiFi.h>         // Biblioteca Wi-Fi para ESP32
#include "secrets.h"

// Configurações do MQTT
#define MQTT_SERVER "homeassistant.local"
#define MQTT_PORT 1883

// Tópicos MQTT

#define DISCOVER_TOPIC "homeassistant/switch/meu_switch/config"
#define COMMAND_TOPIC "meu_switch/command"
#define STATE_TOPIC "meu_switch/state"

#define MQTT_TOPIC_SUB "seu/topico/sub"
#define MQTT_TOPIC_PUB "seu/topico/pub"

// Forward declaration of MQTTManager
class MQTTManager;

// Variáveis globais
extern const char *discovery_topic;
extern const char *command_topic;
extern const char *state_topic;

extern MQTTManager *mqttManager;

class MQTTManager
{
public:
    MQTTManager();
    ~MQTTManager();
    void initMQTT();
    void handleMQTT();
    void publishMessage(const char *topic, const char *payload);
    void publish_discovery();
    void publishSwitchState(bool switch_state);

    inline bool isConnected() { return _mqttClient.connected(); }

private:
    WiFiClient _espClient;
    PubSubClient _mqttClient;

    bool _mqttTaskActive = false;

    static void connectMQTTStatic(void *pvParameters);
    void reconnectMQTT();
};

// Callback para mensagens recebidas
void mqttCallback(char *topic, byte *payload, unsigned int length);

#endif // MQTT_MANAGER_H