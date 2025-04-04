#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// #include "esp_log.h"
#include "utils/utils.h"
#include <Arduino.h> // Se for usar funções do Arduino (digitalWrite, etc.)
#include <LogLibrary.h>

class MotorControl
{
public:
    MotorControl();
    ~MotorControl();
    void begin(uint8_t pinPWM, uint8_t pinDIR, uint8_t pinEN, uint8_t pinSTOP, uint8_t pinSPD, uint8_t pinALM, uint8_t diameter, uint8_t ppr); // Inicializa os pinos
    void setSpeed(int speed);                                                                                                                  // Controla velocidade (0-255)
    void setDirection(bool forward);
    // static void motInt();

private:
    uint8_t _pinPWM, _pinFWR, _pinEN, _pinBRK;
    uint8_t _pinSPD, _pinALM;
    float _speed_mps, _speed_rps;
    uint8_t _diameter, _ppr;
    static volatile uint32_t _speed_pulse;

    static void motInt();
    void calculateSpeed();
};

#endif