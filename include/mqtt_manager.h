#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h> // Biblioteca MQTT
#include <WiFi.h>         // Biblioteca Wi-Fi para ESP32

// Configurações do MQTT
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

// Tópicos MQTT
#define MQTT_TOPIC_SUB "seu/topico/sub"
#define MQTT_TOPIC_PUB "seu/topico/pub"

// Funções públicas
void initMQTT();
void handleMQTT();
void publishMessage(const char* topic, const char* payload);
void reconnectMQTT();

// Callback para mensagens recebidas
void mqttCallback(char* topic, byte* payload, unsigned int length);

#endif // MQTT_MANAGER_H