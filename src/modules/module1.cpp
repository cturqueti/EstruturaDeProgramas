// ---------------------------------------------------------------------------------------------------------------- //
// 
// Descrição: Este arquivo contém a implementação de um módulo específico do projeto.
//
// O que pode ser adicionar:
// Lógica específica do módulo.
// Funções de inicialização e execução.
// 
// ---------------------------------------------------------------------------------------------------------------- //

#include <Arduino.h>
#include "module1.h"
#include "config.h"
#include "pinout.h"
#include "utils.h"

void initModule1() {
    // Inicialização do módulo 1
    pinMode(LED_PIN, OUTPUT);
}

void runModule1() {
    // Lógica do módulo 1
    digitalWrite(LED_PIN, HIGH);
    customDelay(DELAY_SHORT);
    digitalWrite(LED_PIN, LOW);
    customDelay(DELAY_SHORT);
}