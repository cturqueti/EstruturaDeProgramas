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
#include "peripherals.h"
#include <ArduinoOTA.h>
#include "esp_log.h"
#include <ArduinoJson.h>
#include "utils.h"

Peripherals meuPeripherals;
Module1 meuModulo;
Driver1 meuDriver;
WiFiManager meuWiFi;
MQTTManager meuMQTT;

// void connectMQTT(void *pvParameters);

void setup()
{
    meuPeripherals.initPeripherals();
    meuModulo.initModule1();
    meuDriver.initDriver1();
    meuWiFi.initWiFi();

    String device_id = "ESP32_01";
    ComponentConfig led1 = {
        .name = "LED GPIO 2",
        .unique_id = "led_Gpio2",
        .type = ComponentType::SWITCH,
        .gpio = 255,
        .command_topic = generateTopic(device_id, "switch", led1.unique_id, "command"),
        .state_topic = generateTopic(device_id, "switch", led1.unique_id, "state"),
        .callback = [](bool state)
        {
            digitalWrite(2, state);
            Serial.printf("LED 2 alterado para %s\n", state ? "ON" : "OFF");
        }};
    meuMQTT.addComponent(led1);

    ComponentConfig tempSensor = {
        .name = "Temperatura do Quarto",                                                  // 1º campo
        .unique_id = "quarto_Temperature",                                                // 2º campo
        .type = ComponentType::SENSOR,                                                    // 3º campo
        .gpio = 255,                                                                      // 4º campo (opcional, valor padrão)
        .command_topic = "",                                                              // 5º campo (opcional)
        .state_topic = generateTopic(device_id, "sensor", tempSensor.unique_id, "state"), // 6º campo (obrigatório)
        .unit_of_measurement = "°C",                                                      // Campo da struct (ajuste a ordem conforme necessário)
        .device_class = "temperature"                                                     // Novo campo (deve estar após os campos existentes)
    };
    meuMQTT.addComponent(tempSensor);

    ComponentConfig fanConfig = {
        .name = "Ventilador Quarto",
        .unique_id = "quarto_Fan_01",
        .type = ComponentType::FAN,
        .gpio = 2,
        .command_topic = generateTopic(device_id, "fan", fanConfig.unique_id, "power"),
        .state_topic = generateTopic(device_id, "fan", fanConfig.unique_id, "state"),
        .speed_command_topic = generateTopic(device_id, "fan", fanConfig.unique_id, "speed/command"),
        .speed_state_topic = generateTopic(device_id, "fan", fanConfig.unique_id, "speed/state")};
    meuMQTT.addComponent(fanConfig);

    meuMQTT.initMQTT("ESP32_01", "Meu ESP32 Dinâmico"); // o nome só pode ter Maiusculas, minusculas e números
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setPassword(OTA_PASSWORD);
    Serial.println("Iniciando OTA...");
    ArduinoOTA.begin();
}

void loop()
{

    // updateOTA();
    // meuModulo.runModule1();
    // meuDriver.readDriver1();
    ArduinoOTA.handle();
    // Envia uma mensagem a cada 10 segundos
    static unsigned long lastSend = 0;
    if (millis() - lastSend >= 10000)
    {
        // float temp = lerSensorTemperatura(); // Sua função aqui
        float temp = 25.5;
        meuMQTT.publishSensorData("temp_sensor1", temp);
        lastSend = millis();
    }
}
