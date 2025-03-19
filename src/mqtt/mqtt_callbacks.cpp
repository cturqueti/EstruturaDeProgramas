#include "mqtt_manager.h"

// Função de callback para processar mensagens recebidas
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    bool switch_state = false;
    Serial.print("Mensagem recebida no tópico: ");
    Serial.println(topic);

    // Verifica se a mensagem é para o tópico de comando
    if (strcmp(topic, command_topic) == 0)
    {
        if (payload[0] == 'O' && payload[1] == 'N')
        {
            // switch_state = true;
            Serial.println("Switch ligado");
        }
        else if (payload[0] == 'O' && payload[1] == 'F' && payload[2] == 'F')
        {
            // switch_state = false;
            Serial.println("Switch desligado");
        }

        // Publica o estado atual no tópico de estado
        mqttManager->publishSwitchState(switch_state);
    }
}