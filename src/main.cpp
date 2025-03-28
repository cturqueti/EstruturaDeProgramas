// 1️⃣ Definições de pré-processador
#define VERSAO "1.0"
// ---------------------------------------------------------------------------------------------------------------- //
//
// Descrição: Este é o arquivo principal do projeto, onde o programa começa.
//
// O que pode ser adicionar:
// Inicializações de módulos, drivers e tarefas.
// Loop principal do programa.
//
// ---------------------------------------------------------------------------------------------------------------- //

// 2️⃣ Inclusão de bibliotecas
#include "components/Fan.h"
#include "components/Led.h"
#include "components/TemperatureSensor.h"
#include "components/WifiPower.h"
#include "config.h"
#include "esp_log.h"
#include "logging_macros.h"
#include "mqtt/mqtt_manager.h"
#include "ota/ota.h"
#include "peripherals/peripherals.h"
#include "pinout.h"
#include "utils/captive_portal.h"
#include "utils/utils.h"
#include "wifi/wifi_manager.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

// 3️⃣ Declaração de objetos globais
Peripherals meuPeripherals;
WiFiManager meuWiFi;
MQTTManager meuMQTT(DEVICE_ID);

Led led1(meuMQTT, 4);
WifiPower wifiPower(meuMQTT);
Fan meuFan(meuMQTT, 2);
TemperatureSensor tempSensor(meuMQTT, A0, true);

// 4️⃣ Declaração de protótipos de funções
void initializeNormalMode();

// 5️⃣ Função setup()
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

// 6️⃣ Função loop()
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

// 7️⃣ Implementação das funções
void initializeNormalMode()
{
    LOG_INFO("Iniciando NVS...");
    // saveCredentialsToNVS();

    meuWiFi.initWiFi();

    led1.begin();
    wifiPower.begin();
    meuFan.begin();
    tempSensor.begin();

    meuMQTT.initMQTT(meuMQTT.getDeviceId(), "Meu ESP32 Dinâmico"); // o nome só pode ter Maiusculas, minusculas e números
    setupOTA();
}