// #include "mqtt_manager.h"
// #include "pinout.h"

// void handleGpioControl(uint8_t pin, String payload);
// void publishState(uint8_t pin, bool state);

// // Função de callback para processar mensagens recebidas
// void mqttCallback(char *topic, byte *payload, unsigned int length)
// {
//     payload[length] = '\0';
//     String strTopic = String(topic);
//     String strPayload = String((char *)payload);

//     Serial.printf("[MQTT] Mensagem recebida: %s => %s\n", strTopic.c_str(), strPayload.c_str());

//     // Filtro para tópico do LED GPIO 2
//     if (strTopic.endsWith("led1/command"))
//     {
//         handleGpioControl(LED_PIN, strPayload);
//     }
// }

// // Função para controle do GPIO
// void handleGpioControl(uint8_t pin, String payload)
// {
//     payload.toLowerCase(); // Padroniza o payload

//     if (payload == "on" || payload == "1" || payload == "true")
//     {
//         digitalWrite(pin, HIGH);
//         publishState(pin, true); // Confirma o estado
//         Serial.printf("GPIO %d LIGADO\n", pin);
//     }
//     else if (payload == "off" || payload == "0" || payload == "false")
//     {
//         digitalWrite(pin, LOW);
//         publishState(pin, false);
//         Serial.printf("GPIO %d DESLIGADO\n", pin);
//     }
// }

// // Publica o estado atual (feedback)
// void publishState(uint8_t pin, bool state)
// {
//     String topic;

//     switch (pin)
//     {
//     case LED_PIN:
//         topic = "homeassistant/switch/led_gpio2/state";
//         break;
//         // Adicione outros casos conforme necessário
//     }

//     if (topic.length() > 0)
//     {
//         String payload = state ? "ON" : "OFF";
//         // _mqttClient.publish(topic.c_str(), payload.c_str(), true); // retained=true
//     }
// }