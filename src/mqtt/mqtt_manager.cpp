#include "mqtt_manager.h"


MQTTManager::MQTTManager() : _mqttClient(_espClient) {

}

MQTTManager::~MQTTManager() {

}

void MQTTManager::initMQTT() {

    _mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    _mqttClient.setCallback(mqttCallback); // Define a função de callback
}

void MQTTManager::handleMQTT() {
    if (!_mqttClient.connected()) {
        reconnectMQTT();
    }
    _mqttClient.loop(); // Mantém a conexão ativa
}

void MQTTManager::reconnectMQTT() {
    while (!_mqttClient.connected()) {
        Serial.println("Conectando ao broker MQTT...");
        if (_mqttClient.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("Conectado ao broker MQTT!");
            _mqttClient.subscribe(MQTT_TOPIC_SUB); // Inscreve no tópico
        } else {
            Serial.print("Falha na conexão, rc=");
            Serial.print(_mqttClient.state());
            Serial.println(" Tentando novamente em 5 segundos...");
            delay(5000);
        }
    }
}

void MQTTManager::publishMessage(const char* topic, const char* payload) {
    if (_mqttClient.connected()) {
        _mqttClient.publish(topic, payload);
    }
}