#include "mqtt_manager.h"

// Defina as variáveis globais
const char *discovery_topic = DISCOVER_TOPIC;
const char *command_topic = COMMAND_TOPIC;
const char *state_topic = STATE_TOPIC;

MQTTManager *mqttManager = nullptr;

MQTTManager::MQTTManager() : _mqttClient(_espClient)
{
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

void MQTTManager::handleMQTT()
{
    if (!_mqttClient.connected())
    {
        // Verifica se a tarefa MQTT Connect está ativa
        if (!_mqttTaskActive)
        {
            // Cria uma nova tarefa para reconectar
            xTaskCreatePinnedToCore(
                MQTTManager::connectMQTTStatic, // Função estática
                "MQTT Connect",                 // Nome da tarefa
                2048,                           // Tamanho da pilha
                this,                           // Passa o ponteiro da classe como parâmetro
                2,                              // Prioridade da tarefa
                NULL,                           // Handle da tarefa (não usado aqui)
                1                               // Núcleo (core 1)
            );
            _mqttTaskActive = true; // Marca a tarefa como ativa
        }
        else
        {
            // Serial.println("Tarefa MQTT Connect ja estava ativa.");
        }
    }
    _mqttClient.loop(); // Mantém a conexão ativa
}

void MQTTManager::publishMessage(const char *topic, const char *payload)
{
    if (_mqttClient.connected())
    {
        _mqttClient.publish(topic, payload);
    }
}

// Função para publicar a mensagem de descoberta
void MQTTManager::publish_discovery()
{
    String discovery_message = R"({
      "name": "Meu Switch",
      "command_topic": "meu_switch/command",
      "state_topic": "meu_switch/state",
      "unique_id": "meu_switch_esp32",
      "device": {
        "identifiers": ["meu_switch_esp32"],
        "name": "Meu Switch ESP32",
        "manufacturer": "Eu"
      }
    })";

    _mqttClient.publish(discovery_topic, discovery_message.c_str(), true);
    Serial.println("Mensagem de descoberta publicada");
}

void MQTTManager::publishSwitchState(bool switch_state)
{
    _mqttClient.publish(state_topic, switch_state ? "ON" : "OFF", true);
}

void MQTTManager::connectMQTTStatic(void *pvParameters)
{
    // Converte o parâmetro de volta para o tipo MQTTManager*
    MQTTManager *instance = static_cast<MQTTManager *>(pvParameters);
    // Chama a função membro
    instance->reconnectMQTT();
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
            _mqttClient.subscribe(command_topic); // Inscreve no tópico
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