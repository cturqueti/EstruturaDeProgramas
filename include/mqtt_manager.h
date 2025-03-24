#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h> // Biblioteca MQTT
#include <WiFi.h>         // Biblioteca Wi-Fi para ESP32
#include "secrets.h"
#include "ArduinoJson.h"

// Configurações do MQTT
#define MQTT_SERVER "192.168.0.183"
#define MQTT_PORT 1883

// Tópicos MQTT

#define DEVICE_NAME "meu_switch"
#define DEVICE_FRIENDLY_NAME "MeuSwitchESP32"
#define DEVICE_CLASS "motion"
#define DISCOVER_TOPIC "homeassistant/meu_switch/garden/config"
#define COMMAND_TOPIC "homeassistant/meu_switch/garden/command"
#define STATE_TOPIC "homeassistant/meu_switch/garden/state"
#define UNIQUE_ID "meuswitch01ad"
#define IDENTIFIERS "01ad"
#define MANUFACTURER "Eu"

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
    void reconnectMQTT();

private:
    WiFiClient _espClient;
    PubSubClient _mqttClient;

    bool _mqttTaskActive = false;
    String _discoverTopic;
    String _commandTopic;
    String _stateTopic;

    // static void connectMQTTStatic(void *pvParameters);
};

// Callback para mensagens recebidas
void mqttCallback(char *topic, byte *payload, unsigned int length);

#endif // MQTT_MANAGER_H