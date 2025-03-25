#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h> // Biblioteca MQTT
#include <WiFi.h>         // Biblioteca Wi-Fi para ESP32
#include "secrets.h"
#include "ArduinoJson.h"
#include <vector>
#include <queue>
#include <functional>
#include "config_types.h"

// #define MQTT_MAX_PACKET_SIZE 2048

// Configurações do MQTT
#define MQTT_SERVER "homeassistant.local"
#define MQTT_PORT 1883

// Forward declaration of MQTTManager
class MQTTManager;

extern MQTTManager *mqttManager;

class MQTTManager
{
public:
    struct Message
    {
        char topic[128];
        char payload[512];
        bool retained;
    };

    MQTTManager();
    ~MQTTManager();
    void initMQTT(const String &device_id, const String &device_name);
    void addComponent(const ComponentConfig &config);
    void loop();

    void publishSensorData(const String &unique_id, float value);
    void handleMQTT();
    void publishMessage(const char *topic, const char *payload);
    // void publishSwitchState(bool switch_state);

    inline bool isConnected() { return _mqttClient.connected(); }
    void reconnectMQTT();

    void mqttCallback(char *topic, byte *payload, unsigned int length);

    void publishAllDiscoveries();
    void subscribeAllCommandTopics();

private:
    WiFiClient _espClient;
    PubSubClient _mqttClient;
    std::vector<ComponentConfig> _components;
    String _device_id;
    String _device_name;
    bool _mqttTaskActive = false;

    String _discoverTopic;
    String _commandTopic;
    String _stateTopic;
    std::queue<Message> _messageQueue;

    void publishDiscovery(const ComponentConfig &config);
    void handleSwitchMessage(const ComponentConfig &config, const String &payload);
    void handleSensorUpdate(const ComponentConfig &config, bool forceUpdate = false);
    void handleFanMessage(const ComponentConfig &config, const String &payload);
    void handleFanSpeedMessage(const ComponentConfig &config, const String &payload);

    // static void connectMQTTStatic(void *pvParameters);
};

#endif // MQTT_MANAGER_H