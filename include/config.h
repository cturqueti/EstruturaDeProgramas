// ---------------------------------------------------------------------------------------------------------------- //
//
// Este arquivo contém configurações globais do projeto, como constantes, macros, e configurações de hardware.
//
// O que pode ser adicionado:
// Constantes de tempo (delays, timeouts).
// Configurações de debug (por exemplo, ativar/desativar logs).
// Configurações de hardware (por exemplo, tamanho de buffers).
//
// ---------------------------------------------------------------------------------------------------------------- //

#ifndef CONFIG_H
#define CONFIG_H

// Configurações de debug
#define DEBUG_ENABLED 1

// Constantes de tempo
#define DELAY_SHORT 100
#define DELAY_LONG 1000

// Tamanho de buffers
#define BUFFER_SIZE 128

#define OTA_PORT 3232

#define HOSTNAME "esp32test"

#define SERIAL_BAUD 115200

#endif // CONFIG_H