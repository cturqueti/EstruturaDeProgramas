#include "mqtt_manager.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void initMQTT() {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback); // Define a função de callback
}

void handleMQTT() {
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop(); // Mantém a conexão ativa
}

void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.println("Conectando ao broker MQTT...");
        if (mqttClient.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("Conectado ao broker MQTT!");
            mqttClient.subscribe(MQTT_TOPIC_SUB); // Inscreve no tópico
        } else {
            Serial.print("Falha na conexão, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" Tentando novamente em 5 segundos...");
            delay(5000);
        }
    }
}

void publishMessage(const char* topic, const char* payload) {
    if (mqttClient.connected()) {
        mqttClient.publish(topic, payload);
    }
}