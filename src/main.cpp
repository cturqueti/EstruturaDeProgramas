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
#include "components/Led.h"
#include "components/WifiPower.h"
#include "components/Fan.h"
#include "components/TemperatureSensor.h"

Peripherals meuPeripherals;
Module1 meuModulo;
Driver1 meuDriver;
WiFiManager meuWiFi;
MQTTManager meuMQTT(DEVICE_ID);

Led led1(meuMQTT, 4);
WifiPower wifiPower(meuMQTT);
Fan meuFan(meuMQTT, 2);
TemperatureSensor tempSensor(meuMQTT, A0, true);

// void connectMQTT(void *pvParameters);

void setup()
{

    meuPeripherals.initPeripherals();
    meuModulo.initModule1();
    meuDriver.initDriver1();
    meuWiFi.initWiFi();

    led1.begin();
    wifiPower.begin();
    meuFan.begin();
    tempSensor.begin();

    meuMQTT.initMQTT(DEVICE_ID, "Meu ESP32 Dinâmico"); // o nome só pode ter Maiusculas, minusculas e números
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setPassword(OTA_PASSWORD);
    Serial.println("Iniciando OTA...");
    ArduinoOTA.begin();
}

void loop()
{
    // updateOTA();

    ArduinoOTA.handle();

    static unsigned long lastSend = millis();
    if (millis() - lastSend >= 10000)
    {

        tempSensor.update();

        wifiPower.update();

        lastSend = millis();
    }
}
