#include "mqtt_manager.h"

// Função de callback para processar mensagens recebidas
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Mensagem recebida no tópico: ");
    Serial.println(topic);

    // Converte o payload para uma string
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    Serial.print("Payload: ");
    Serial.println(message);

    // Aqui você pode adicionar a lógica para processar a mensagem
    // Por exemplo, acionar um relé, mudar o estado de um LED, etc.
}