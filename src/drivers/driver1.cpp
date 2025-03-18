// ---------------------------------------------------------------------------------------------------------------- //
// 
// Descrição: Este arquivo contém a implementação de um driver para hardware específico.
//
// O que pode ser adicionar:
// Inicialização do driver.
// Funções para interagir com o hardware.
// 
// ---------------------------------------------------------------------------------------------------------------- //

#include "driver1.h"
#include "pinout.h"
#include <Arduino.h>

void initDriver1() {
    // Inicialização do driver 1
    pinMode(BUTTON_PIN, INPUT);
}

bool readDriver1() {
    // Leitura do driver 1
    return digitalRead(BUTTON_PIN);
}