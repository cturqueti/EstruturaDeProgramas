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

Module1 meuModulo;
Driver1 meuDriver;
WiFiManager meuWiFi;

void setup() {
    meuModulo.initModule1();
    meuDriver.initDriver1();
    meuWiFi.initWiFi();
}

void loop() {
    meuModulo.runModule1();
    meuDriver.readDriver1();
    meuWiFi.handleWiFi();
}