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

    // 2. Adiciona componentes dinamicamente
    ComponentConfig led1 = {
        "LED GPIO 2",
        "led_gpio2",
        ComponentType::SWITCH,
        255,
        "homeassistant/switch/led_gpio2/command",
        "homeassistant/switch/led_gpio2/state",
        [](bool state)
        {
            digitalWrite(2, state);
            Serial.printf("LED 2 alterado para %s\n", state ? "ON" : "OFF");
        }};
    meuMQTT.addComponent(led1);

    ComponentConfig tempSensor = {
        "Sensor Temperatura",
        "temp_sensor1",
        ComponentType::SENSOR,
        255, // Não usa GPIO
        "",  // Sem command topic
        "homeassistant/sensor/temp_sensor1/state"};
    meuMQTT.addComponent(tempSensor);

    ComponentConfig fanConfig;
    fanConfig.type = ComponentType::FAN;
    fanConfig.gpio = 2;              // GPIO 3
    fanConfig.pwm_channel = 0;       // Canal PWM 0
    fanConfig.pwm_frequency = 25000; // 25kHz (frequência comum para fans)
    fanConfig.pwm_resolution = 8;    // 8 bits (0-255)
    fanConfig.name = "Ventilador Quarto";
    fanConfig.unique_id = "quarto_fan_01";
    fanConfig.command_topic = "home/bedroom/fan/power";
    fanConfig.state_topic = "home/bedroom/fan/state";
    fanConfig.speed_command_topic = "home/bedroom/fan/speed/set";
    fanConfig.speed_state_topic = "home/bedroom/fan/speed/state";
    fanConfig.speeds = "off,low,medium,high"; // Opcional, para controle discreto

    mqttManager->addComponent(fanConfig);

    meuMQTT.initMQTT("ESP32_01", "Meu ESP32 Dinâmico");
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
