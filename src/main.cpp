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
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "peripherals.h"
#include <ArduinoOTA.h>
#include "ota.h"
#include "esp_log.h"
#include <ArduinoJson.h>
#include "utils.h"
#include "logging_macros.h"
#include "components/Led.h"
#include "components/WifiPower.h"
#include "components/Fan.h"
#include "components/TemperatureSensor.h"
#include "captive_portal.h"
#include "FS.h"

Peripherals meuPeripherals;
WiFiManager meuWiFi;
MQTTManager meuMQTT(DEVICE_ID);

Led led1(meuMQTT, 4);
WifiPower wifiPower(meuMQTT);
Fan meuFan(meuMQTT, 2);
TemperatureSensor tempSensor(meuMQTT, A0, true);

void initializeNormalMode();

// void connectMQTT(void *pvParameters);

void setup()
{
    Utils::enableLogColors(false);
    Utils::setLogLevel(LogLevel::DEBUG_ALL);
    meuPeripherals.initPeripherals();

    // Inicializa SPIFFS para o HTML
    checkCredentials();
    if (shouldStartPortal)
    {
        startConfigPortal();
    }
    else
    {
        initializeNormalMode();
    }
}

void initializeNormalMode()
{
    LOG_INFO("Iniciando NVS...");
    saveCredentialsToNVS();

    meuWiFi.initWiFi();

    led1.begin();
    wifiPower.begin();
    meuFan.begin();
    tempSensor.begin();

    meuMQTT.initMQTT(meuMQTT.getDeviceId(), "Meu ESP32 Dinâmico"); // o nome só pode ter Maiusculas, minusculas e números
    setupOTA();
}

void loop()
{
    if (isPortalActive())
    {
        handlePortal();
    }
    else
    {
        ArduinoOTA.handle();

        static unsigned long lastSend = millis();
        if (millis() - lastSend >= 10000)
        {
            tempSensor.update();
            wifiPower.update();
            lastSend = millis();
        }
    }
}
