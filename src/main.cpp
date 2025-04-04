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
#include <LittleFS.h>
#include <LogLibrary.h>

// 3️⃣ Declaração de objetos globais
Peripherals meuPeripherals;
WiFiManager meuWiFi;
MQTTManager meuMQTT(DEVICE_ID);
CaptivePortal meuPortal;

Led led1(meuMQTT, 4);
WifiPower wifiPower(meuMQTT);
Fan meuFan(meuMQTT, 2);
TemperatureSensor tempSensor(meuMQTT, A0, true);
Ota meuOta;

// 4️⃣ Declaração de protótipos de funções
void initializeNormalMode();

// 5️⃣ Função setup()
void setup()
{
    LittleFS.begin();
    Serial.begin(9600);
    Log::begin(&Serial, 1024);
    Log::enableColors(false);
    Log::setLogLevel(LogLevel::INFO); // DEBUG_ALL, INFO_AND_ABOVE, WARNING_AND_ABOVE, ERROR_ONLY
    meuPeripherals.initPeripherals();

    // Verifica se tem credenciais salvas
    if (!isCredentials())
    // if (true)
    {
        meuPortal.startCaptivePortal(); // Inicia o portal
    }
    else
    {
        initializeNormalMode(); // Inicia o modo normal
    }
}

// 6️⃣ Função loop()
void loop()
{
    if (meuPortal.isPortalActive())
    {
        // meuPortal.handlePortal();
    }
    else
    {

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

    meuWiFi.beginClient(); // inicia wifi e task para verificar o estado

    led1.begin();
    wifiPower.begin();
    meuFan.begin();
    tempSensor.begin();

    meuMQTT.initMQTT(meuMQTT.getDeviceId(), "Meu ESP32 Dinâmico"); // inicia MQTT e task para verificar mensagens
    meuOta.begin();
}