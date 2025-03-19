// ---------------------------------------------------------------------------------------------------------------- //
//
// Descrição: Este arquivo contém a implementação de um módulo específico do projeto.
//
// O que pode ser adicionar:
// Lógica específica do módulo.
// Funções de inicialização e execução.
//
// ---------------------------------------------------------------------------------------------------------------- //

#include "module1.h"
#include "Arduino.h"
#include "config.h"
#include "pinout.h"
#include "utils.h"

Module1::Module1()
{
    // Construtor
}

Module1::~Module1()
{
    // Destrutor
}

void Module1::initModule1()
{
    // Inicialização do módulo 1
    Serial.println("Iniciando o módulo 1...");
    pinMode(LED_PIN, OUTPUT);
    ledStatusMain = false;
}

void Module1::runModule1()
{
    // Lógica do módulo 1
    digitalWrite(LED_PIN, ledStatusMain);
    Utils::customDelay(DELAY_LONG);
    ledStatusMain = !ledStatusMain;
}