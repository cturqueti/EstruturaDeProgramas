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
#include "Arduino.h"
#include "config.h"
#include "pinout.h"

Driver1::Driver1()
{
    // Construtor
}

Driver1::~Driver1()
{
    // Destrutor
}

void Driver1::initDriver1()
{
    // Inicialização do driver 1
    Serial.println("Iniciando o driver 1...");
}

bool Driver1::readDriver1()
{
    // Leitura do driver 1
    return true;
}