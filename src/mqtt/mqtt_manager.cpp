#include "mqtt_manager.h"
#include "pinout.h"
#include "utils.h"

// Defina as variáveis globais
MQTTManager *mqttManager = nullptr;

MQTTManager::MQTTManager(const String &deviceId) : _mqttClient(_espClient), _device_id(deviceId)
{
    _mqttClient.setBufferSize(2048);
    _availabilityTopic = generateTopic(_device_id, "status");
}

MQTTManager::~MQTTManager()
{
}

void MQTTManager::initMQTT(const String &device_id, const String &device_name)
{
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(2000);
        Serial.println("MQTT aguardando Wi-Fi...");
    }
    Serial.println("MQTT liberado");
    _mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    _device_id = device_id;
    _device_name = device_name;

    _mqttClient.setCallback([this](char *topic, byte *payload, unsigned int length)
                            { this->mqttCallback(topic, payload, length); });

    handleMQTT();
}

void MQTTManager::addComponent(const ComponentConfig &config)
{
    _components.push_back(config);

    // Configura GPIO se for saída
    switch (config.type)
    {
    case ComponentType::SWITCH:

        if (config.gpio != 255)
        {
            Serial.printf("[DEBUG] Configurando GPIO %d como saída\n", config.gpio);
            pinMode(config.gpio, OUTPUT);
            digitalWrite(config.gpio, LOW);
        }
        break;

    case ComponentType::FAN:

        if (config.gpio != 255)
        {
            Serial.printf("[DEBUG] Configurando GPIO %d como saída\n", config.gpio);
            pinMode(config.gpio, OUTPUT);
            analogWrite(config.gpio, 0);
        }
        break;

    default:
        break;
    }
}

void MQTTManager::loop()
{
    _mqttClient.loop();
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 30000)
    { // A cada 30 segundos
        _mqttClient.publish(generateTopic(_device_id, "status").c_str(), "online", true);
        lastUpdate = millis();
    }
}

void MQTTManager::publishSensorData(const String &unique_id, float value)
{
    for (const auto &component : _components)
    {
        if (component.unique_id == unique_id && component.type == ComponentType::SENSOR)
        {
            _mqttClient.publish(component.state_topic.c_str(), String(value).c_str(), true);
            Serial.printf("Publicando sensor %s: %f\n", component.name.c_str(), value);
        }
    }
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

void MQTTManager::reconnectMQTT()
{
    while (!_mqttClient.connected() && WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Conectando ao broker MQTT...");
        if (_mqttClient.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD))
        {
            Serial.println("Conectado ao broker MQTT!");

            // Publica todas as mensagens de discovery
            publishAllDiscoveries();

            for (const auto &component : _components)
            {
                if (component.type == ComponentType::FAN)
                {
                    _mqttClient.publish(component.state_topic.c_str(), "false", true);
                    _mqttClient.publish(component.speed_state_topic.c_str(), "0", true);
                    //_mqttClient.publish(component.preset_mode_state_topic.c_str(), "auto", true);
                }
            }

            // Inscreve em todos os tópicos de comando
            subscribeAllCommandTopics();

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

void MQTTManager::publishAllDiscoveries()
{
    for (const auto &component : _components)
    {
        publishDiscovery(component);
        delay(100);
    }
}

void MQTTManager::subscribeAllCommandTopics()
{
    for (const auto &component : _components)
    {
        if ((component.type == ComponentType::SWITCH || component.type == ComponentType::FAN) && !component.command_topic.isEmpty())
        {
            _mqttClient.subscribe(component.command_topic.c_str());
            Serial.printf("Inscrito no tópico: %s\n", component.command_topic.c_str());
        }
        if (component.type == ComponentType::FAN && !component.speed_command_topic.isEmpty())
        {
            _mqttClient.subscribe(component.speed_command_topic.c_str());
            Serial.printf("Inscrito no tópico de velocidade: %s\n", component.speed_command_topic.c_str());
        }
    }
}

// -------------------- Private Methods --------------------
void connectMQTTStatic(void *pvParameters)
{
    // Converte o parâmetro de volta para o tipo MQTTManager*
    MQTTManager *instance = static_cast<MQTTManager *>(pvParameters);
    // Chama a função membro
    instance->reconnectMQTT();
    while (true)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        if (!instance->isConnected())
        {
            instance->reconnectMQTT();
            Serial.println("Reconectado ao broker MQTT!");
        }
        else
        {
            instance->loop();
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

void MQTTManager::mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.println("------------- MQTT CALLBACK ---------------");

    // Garante terminação nula para o payload
    char payloadStr[length + 1];
    memcpy(payloadStr, payload, length);
    payloadStr[length] = '\0';
    bool state = 0;

    String strTopic = String(topic);
    String strPayload = String(payloadStr);

    Serial.printf("[MQTT] Mensagem recebida - Tópico: %s, Payload: %s\n",
                  strTopic.c_str(), strPayload.c_str());

    for (const auto &component : _components)
    {
        if (component.command_topic.equals(strTopic))
        {
            switch (component.type)
            {
            case ComponentType::SWITCH:
                state = (strPayload == "ON" || strPayload == "1");
                component.callback(state, component.context);
                // handleSwitchMessage(component, strPayload);
                return;

            case ComponentType::FAN:
                state = (strPayload == "ON" || strPayload == "1");
                component.callback(state, component.context);
                return;
            }
        }
        else if (component.type == ComponentType::FAN &&
                 component.speed_command_topic.equals(strTopic) &&
                 component.speed_callback)
        {
            int speed = strPayload.toInt();
            component.speed_callback(speed, component.context);
            return;
        }
    }

    Serial.println("Nenhum componente correspondente encontrado para o tópico");
    Serial.println("-------------------------------------------");
}

void MQTTManager::publishDiscovery(const ComponentConfig &config)
{

    JsonDocument doc;

    // Device info (comum a todos os componentes)
    JsonObject device = doc["device"].to<JsonObject>();
    JsonArray identifiers = device["identifiers"].to<JsonArray>();
    identifiers.add(_device_id); // Deve ser array mesmo com um único ID
    device["name"] = _device_name;
    device["model"] = "ESP32";
    device["manufacturer"] = "Sideout";
    device["sw_version"] = "1.0.0";
    // device["configuration_url"] = "http://" + WiFi.localIP().toString();

    doc["name"] = config.name;
    doc["unique_id"] = config.unique_id;
    doc["availability_topic"] = _availabilityTopic;

    switch (config.type)
    {
    case ComponentType::SWITCH:
        doc["command_topic"] = config.command_topic;
        doc["state_topic"] = config.state_topic;
        doc["payload_on"] = "ON";
        doc["payload_off"] = "OFF";
        doc["optimistic"] = false;
        break;

    case ComponentType::FAN:
    {
        doc["command_topic"] = config.command_topic;
        doc["state_topic"] = config.state_topic;
        doc["percentage_command_topic"] = config.speed_command_topic;
        doc["percentage_state_topic"] = config.speed_state_topic;
        doc["payload_on"] = "true";
        doc["payload_off"] = "false";

        // Configurações numéricas
        doc["percentage_step"] = 1;
        doc["speed_range_min"] = 1;
        doc["speed_range_max"] = 100;

        // Preset modes como array
        JsonArray preset_modes = doc["preset_modes"].to<JsonArray>();
        preset_modes.add("auto");
        preset_modes.add("smart");
        break;
    }

    case ComponentType::SENSOR:
        doc["state_topic"] = config.state_topic;
        if (!config.unit_of_measurement.isEmpty())
        {
            doc["unit_of_measurement"] = config.unit_of_measurement;
        }
        if (!config.device_class.isEmpty())
        {
            doc["device_class"] = config.device_class;
        }
        break;
    }

    // Determina o tipo correto para o tópico
    String component_type;
    switch (config.type)
    {
    case ComponentType::SWITCH:
        component_type = "switch";
        break;
    case ComponentType::FAN:
        component_type = "fan";
        break;
    case ComponentType::SENSOR:
        component_type = "sensor";
        break;
    default:
        component_type = "sensor";
        break;
    }

    // doc["device_class"] = component_type;

    String payload;
    serializeJson(doc, payload);

#ifdef DEBUG_ENABLED
    Serial.printf("[DEBUG] Tentando publicar no tópico: %s\n", generateTopic(_device_id, component_type, config.unique_id, "config").c_str());
    Serial.println(payload);
    Serial.printf("[DEBUG] Tamanho do payload: %d bytes\n", payload.length());
#endif // DEBUG

    bool published = _mqttClient.publish(generateTopic(_device_id, component_type, config.unique_id, "config").c_str(), payload.c_str(), true);

    if (!published)
    {
        Serial.printf("[ERRO] Falha ao publicar. Estado do MQTT: %d\n", _mqttClient.state());
#ifdef DEBUG_ENABLED
        Serial.printf("[ERRO] Tamanho máximo do buffer: %d\n", _mqttClient.getBufferSize());
#endif // DEBUG
    }
    else
    {
        _mqttClient.publish(generateTopic(_device_id, "status").c_str(), "online", true);
    }
}

void MQTTManager::handleFanMessage(const ComponentConfig &config, const String &payload)
{
    bool state = (payload == "ON" || payload == "1");

    if (state && config.gpio != 255)
    {
        int speed = config.last_speed > 0 ? config.last_speed : 50;
        analogWrite(config.gpio, map(speed, 0, 100, 0, 255));
    }
    else if (config.gpio != 255)
    {
        analogWrite(config.gpio, 0);
    }

    if (!config.state_topic.isEmpty())
    {
        _mqttClient.publish(config.state_topic.c_str(), state ? "ON" : "OFF", true);
    }

    if (config.callback)
    {
        config.callback(state, config.context); // Passando contexto
    }
}

void MQTTManager::handleFanSpeedMessage(const ComponentConfig &config, const String &payload)
{
    int speed = payload.toInt();
    speed = constrain(speed, 0, 100); // Garante que está entre 0-100%

    // Mapeia para 0-255 (8 bits)
    int pwmValue = map(speed, 0, 100, 0, 255);
    analogWrite(config.gpio, pwmValue);

    // Publica o estado da velocidade
    if (!config.speed_state_topic.isEmpty())
    {
        _mqttClient.publish(config.speed_state_topic.c_str(), String(speed).c_str(), true);
    }

    // Se tinha um callback de velocidade, executa
    if (config.speed_callback)
    {
        config.speed_callback(speed, config.context);
    }
}