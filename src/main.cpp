// ---------------------------------------------------------------------------------------------------------------- //
//
// Descrição: Este é o arquivo principal do projeto, onde o programa começa.
//
// O que pode ser adicionar:
// Inicializações de módulos, drivers e tarefas.
// Loop principal do programa.
//
// ---------------------------------------------------------------------------------------------------------------- //

#include "Arduino.h"
#include "config.h"
#include "pinout.h"
#include "module1.h"
#include "driver1.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include <ArduinoOTA.h>
#include "esp_log.h"

Module1 meuModulo;
Driver1 meuDriver;
WiFiManager meuWiFi;
MQTTManager meuMQTT;

void setup()
{
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI("TAG", "Olá, mundo!");
    meuModulo.initModule1();
    meuDriver.initDriver1();
    meuWiFi.initWiFi();
    meuMQTT.initMQTT();
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setPassword(OTA_PASSWORD);
    ArduinoOTA.begin();
}

void loop()
{
    // updateOTA();
    meuModulo.runModule1();
    meuDriver.readDriver1();
    meuWiFi.handleWiFi();
    meuMQTT.handleMQTT();
    ArduinoOTA.handle();
    // Envia uma mensagem a cada 10 segundos
    static unsigned long lastSend = 0;
    if (millis() - lastSend >= 10000)
    {
        lastSend = millis();
        meuMQTT.publishMessage(MQTT_TOPIC_PUB, "Mensagem enviada a cada 10 segundos");
    }
}

// void updateOTA()
// {
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(1000);
//         Serial.println("Conectando ao Wi-Fi...");
//     }
//     Serial.println("Conectado ao Wi-Fi!");
//     ArduinoOTA.begin(OTA_PORT, OTA_PASSWORD);
// }