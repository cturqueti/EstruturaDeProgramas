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
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(2000);
        Serial.println("MQTT aguardando Wi-Fi...");
    }
    Serial.println("MQTT liberado");
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
    // <discovery_prefix>/<component>/[<node_id>/]<object_id>/config
    // Cria um objeto JSON
    JsonDocument jsonDoc;
    String topic_address;

    JsonVariant device = jsonDoc["dev"].to<JsonObject>();
    JsonVariant origin = jsonDoc["0"].to<JsonObject>();
    JsonVariant components = jsonDoc["cmps"].to<JsonObject>();

    device["ids"] = IDENTIFIERS;
    device["name"] = DEVICE_FRIENDLY_NAME;
    device["sw"] = "1.0";
    device["mdl"] = DEVICE_MODEL;
    device["mf"] = DEVICE_MANUFACTURER;
    device["swv"] = DEVICE_SW_VERSION;

    origin["Name"] = DEVICE_NAME;
    origin["Sw"] = "1.0";

    // Adiciona os campos ao JSON

    jsonDoc["command_topic"] = COMMAND_TOPIC;
    jsonDoc["state_topic"] = STATE_TOPIC;

    // JsonVariant device = jsonDoc["device"].to<JsonObject>();
    // device["identifiers"][0] = IDENTIFIERS;
    // device["name"] = DEVICE_FRIENDLY_NAME;

    // Converte o JSON para uma string
    String discovery_message;
    serializeJson(jsonDoc, discovery_message);
    Serial.println("Tamanho da mensagem: " + String(strlen(discovery_message.c_str())));

    _discoverTopic = "homeassistant/switch/" + String(DEVICE_NAME) + "/" + String(IDENTIFIERS) + "/config";
    // Publica a mensagem de descoberta
    bool result = _mqttClient.publish(_discoverTopic.c_str(), discovery_message.c_str(), true);
    // bool result = _mqttClient.publish(DISCOVER_TOPIC, "teste", true);
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
    while (!_mqttClient.connected() && WiFi.status() == WL_CONNECTED)
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