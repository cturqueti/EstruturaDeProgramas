#include "mqtt_manager.h"

// Defina as variáveis globais
MQTTManager *mqttManager = nullptr;

MQTTManager::MQTTManager() : _mqttClient(_espClient)
{
    // Gera os tópicos dinamicamente
    _discoverTopic = "homeassistant/switch/" + String(DEVICE_NAME) + "/config";
    _commandTopic = String(DEVICE_NAME) + "/command";
    _stateTopic = String(DEVICE_NAME) + "/state";
}

MQTTManager::~MQTTManager()
{
}

void MQTTManager::initMQTT()
{
    _mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    _mqttClient.setCallback(mqttCallback);
    handleMQTT();
}

void connectMQTTStatic(void *pvParameters)
{
    // Converte o parâmetro de volta para o tipo MQTTManager*
    MQTTManager *instance = static_cast<MQTTManager *>(pvParameters);
    // Chama a função membro
    instance->reconnectMQTT();
    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (!instance->isConnected())
        {
            instance->reconnectMQTT();
            Serial.println("Reconectado ao broker MQTT!");
        }
    }
    vTaskDelete(nullptr);
}

void MQTTManager::handleMQTT()
{
    if (!_mqttClient.connected())
    {
        Serial.println("Conectando ao broker MQTT...");
        // Verifica se a tarefa MQTT Connect está ativa
        if (!_mqttTaskActive)
        {
            // Cria uma nova tarefa para reconectar
            BaseType_t result = xTaskCreate(connectMQTTStatic, "MQTT Connect", 8192, this, 2, NULL);
            Serial.println("Tarefa de MQTT criada...");
            _mqttTaskActive = true; // Marca a tarefa como ativa
        }
    }
    _mqttClient.loop(); // Mantém a conexão ativa
}

void MQTTManager::publishMessage(const char *topic, const char *payload)
{
    if (_mqttClient.connected())
    {
        bool result = _mqttClient.publish(topic, payload);
        Serial.printf("Conectado ao broker MQTT: %s:%d\n", MQTT_SERVER, MQTT_PORT);
        if (result)
        {
            Serial.printf("Mensagem publicada com sucesso no tópico %s: %s\n", topic, payload);
        }
        else
        {
            Serial.printf("Falha ao publicar mensagem no tópico %s\n", topic);
        }
    }
    else
    {
        Serial.printf("Cliente MQTT não conectado. Não foi possível publicar a mensagem: %s\n", payload);
    }
}

// Função para publicar a mensagem de descoberta
void MQTTManager::publish_discovery()
{
    // Cria um objeto JSON
    JsonDocument jsonDoc;

    // Adiciona os campos ao JSON
    jsonDoc["name"] = nullptr;
    jsonDoc["command_topic"] = DEVICE_CLASS;
    jsonDoc["state_topic"] = STATE_TOPIC;
    jsonDoc["unique_id"] = UNIQUE_ID;

    JsonVariant device = jsonDoc.createNestedObject("device");
    device["identifiers"][0] = IDENTIFIERS;
    device["name"] = DEVICE_FRIENDLY_NAME;

    // Converte o JSON para uma string
    String discovery_message;
    serializeJson(jsonDoc, discovery_message);

    // Publica a mensagem de descoberta
    bool result = _mqttClient.publish(DISCOVER_TOPIC, discovery_message.c_str(), true);
    if (result)
    {
        Serial.printf("Mensagem publicada com sucesso no tópico %s: %s\n", _discoverTopic.c_str(), discovery_message.c_str());
    }
    else
    {
        Serial.printf("Falha ao publicar mensagem no tópico %s\n", _discoverTopic.c_str());
    }
}

void MQTTManager::publishSwitchState(bool switch_state)
{
    _mqttClient.publish(_stateTopic.c_str(), switch_state ? "ON" : "OFF", true);
}

void MQTTManager::reconnectMQTT()
{
    while (!_mqttClient.connected())
    {
        Serial.println("Conectando ao broker MQTT...");
        if (_mqttClient.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD))
        {
            Serial.println("Conectado ao broker MQTT!");
            publish_discovery();
            Serial.println("Inscrevendo em um topico...");
            _mqttClient.subscribe(_commandTopic.c_str()); // Inscreve no tópico
            _mqttTaskActive = false;
        }
        else
        {
            Serial.print("Falha na conexão, rc=");
            Serial.print(_mqttClient.state());
            Serial.println(" Tentando novamente em 5 segundos...");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }
}