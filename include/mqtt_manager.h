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
    MQTTManager();
    ~MQTTManager();

    void initMQTT(const String &device_id, const String &device_name);
    void addComponent(const ComponentConfig &config);
    void loop();
    void publishSensorData(const String &unique_id, float value);
    void publishMessage(const char *topic, const char *payload);
    inline bool isConnected() { return _mqttClient.connected(); }

    void reconnectMQTT();

private:
    WiFiClient _espClient;
    PubSubClient _mqttClient;
    String _device_id;
    String _device_name;
    std::vector<ComponentConfig> _components;
    bool _mqttTaskActive = false;

    void handleMQTT();

    void mqttCallback(char *topic, byte *payload, unsigned int length);
    void publishAllDiscoveries();
    void subscribeAllCommandTopics();
    void publishDiscovery(const ComponentConfig &config);

    void handleFanMessage(const ComponentConfig &config, const String &payload);
    void handleFanSpeedMessage(const ComponentConfig &config, const String &payload);
};

void connectMQTTStatic(void *pvParameters);

#endif // MQTT_MANAGER_H