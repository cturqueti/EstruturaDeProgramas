// ---------------------------------------------------------------------------------------------------------------- //
// 
// Descrição: Este é o arquivo principal do projeto, onde o programa começa.
// 
// O que pode ser adicionar:
// Inicializações de módulos, drivers e tarefas.
// Loop principal do programa.
// 
// ---------------------------------------------------------------------------------------------------------------- //

#include <Arduino.h>
#include "config.h"
#include "pinout.h"
#include "peripherals.h"
#include "utils.h"
#include "mqtt_manager.h"
#include "modules/module1.h"
#include "drivers/driver1.h"
#include "tasks/task1.h"
#include "wifi/wifi_manager.h"

void setup() {
    // Inicializações
    initPeripherals();
    initDriver1();
    initModule1();
    initTask1();
    initWiFi();
    initMQTT();
}

void loop() {
    // Loop principal
    runTask1();
    runModule1();
    handleWiFi();   // Mantém a conexão Wi-Fi
    handleMQTT();   // Mantém a conexão MQTT e processa mensagens

    // Exemplo: Publica uma mensagem a cada 10 segundos
    static unsigned long lastPublish = 0;
    if (millis() - lastPublish > 10000) {
        lastPublish = millis();
        publishMessage(MQTT_TOPIC_PUB, "Olá, MQTT!");
    }
}