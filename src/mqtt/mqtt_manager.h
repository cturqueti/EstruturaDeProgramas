#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "config.h"
#include "config_types.h"
#include "utils/utils.h"
#include <ArduinoJson.h>
#include <LogLibrary.h>
#include <PubSubClient.h> // Biblioteca MQTT
#include <WiFi.h>         // Biblioteca Wi-Fi para ESP32
#include <functional>
#include <mutex>
#include <queue>
#include <vector>

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
    MQTTManager(const String &deviceId);
    ~MQTTManager();

    std::mutex mqttInstanceMutex;

    void initMQTT(const String &device_id, const String &device_name);
    void addComponent(const ComponentConfig &config);
    void loop();
    void publishSensorData(const String &unique_id, float value);
    void publishMessage(const char *topic, const char *payload);
    inline bool isConnected() { return _mqttClient.connected(); }
    inline String getDeviceId() { return _device_id; }
    inline bool isMqttActive() const { return _mqttTaskActive; }

    /**
     * @brief Retorna uma instância única do gerenciador MQTT
     *
     * Caso não haja uma instância anterior, cria uma nova com o deviceId
     * passado como par metro. Caso o deviceId seja vazio, reutiliza a instância
     * anterior.
     *
     * @param deviceId ID do dispositivo a ser usado na conex o MQTT
     * @return Instância do gerenciador MQTT
     */
    static MQTTManager *getInstance(const String &deviceId)
    {
        if (!instance && !deviceId.isEmpty())
        {
            instance = new MQTTManager(deviceId);
        }
        return instance;
    }

    bool reconnectMQTT();

private:
    WiFiClient _espClient;
    PubSubClient _mqttClient;
    Topic _topic;
    String _device_id;
    String _device_name;
    String _availabilityTopic;
    std::vector<ComponentConfig> _components;
    bool _mqttTaskActive = false;

    static MQTTManager *instance;

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