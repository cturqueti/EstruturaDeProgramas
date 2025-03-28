// ---------------------------------------------------------------------------------------------------------------- //
//
// Descrição: Este arquivo contém definições relacionadas aos periféricos do microcontrolador.
//
// O que pode ser adicionado:
// Inicializações de periféricos (UART, SPI, I2C).
// Funções para manipulação de periféricos.
//
// ---------------------------------------------------------------------------------------------------------------- //

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <HardwareSerial.h>
#include "config.h"

class Peripherals
{
public:
    void initPeripherals(); // Declaração da função
};

#endif // PERIPHERALS_H