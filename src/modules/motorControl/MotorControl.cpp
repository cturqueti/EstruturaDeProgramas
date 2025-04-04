#include "MotorControl.h"

volatile uint32_t MotorControl::_speed_pulse = 0;

MotorControl::MotorControl()
{
}

MotorControl::~MotorControl()
{
}

void MotorControl::begin(uint8_t pinPWM, uint8_t pinFWR, uint8_t pinEN, uint8_t pinBRK, uint8_t pinSPD, uint8_t pinALM, uint8_t diameter, uint8_t ppr)
{
    _pinPWM = pinPWM;
    _pinFWR = pinFWR;
    _pinEN = pinEN;
    _pinBRK = pinBRK;
    _pinSPD = pinSPD;
    _pinALM = pinALM;

    _pinSPD = pinSPD;
    _diameter = diameter;
    _ppr = ppr;

    pinMode(_pinPWM, OUTPUT);
    pinMode(_pinFWR, OUTPUT);
    pinMode(_pinBRK, OUTPUT);
    pinMode(_pinEN, OUTPUT);
    pinMode(_pinSPD, INPUT);
    pinMode(_pinALM, INPUT);

    attachInterrupt(digitalPinToInterrupt(_pinSPD), []
                    { MotorControl::motInt(); }, RISING);
}

void MotorControl::setSpeed(int speed)
{
    speed = constrain(speed, 0, 255); // Garante valor entre 0 e 255
    analogWrite(_pinPWM, speed);
}

void MotorControl::setDirection(bool forward)
{
    digitalWrite(_pinFWR, forward ? HIGH : LOW);
}

void MotorControl::calculateSpeed()
{
    static uint32_t last_pulse = 0;
    static uint32_t last_time = millis();

    uint32_t current_pulse = _speed_pulse;
    uint32_t current_time = millis();

    // Calcula diferença de pulsos e tempo
    uint32_t delta_pulse = current_pulse - last_pulse;
    float delta_time = (current_time - last_time) / 1000.0; // Converte ms para s

    // Evita divisão por zero (se delta_time for muito pequeno)
    if (delta_time > 0)
    {
        // Circunferência em metros: π * diâmetro (mm) / 1000
        float circumference_m = PI * _diameter / 1000.0;

        // Cálculo da velocidade em m/s
        float speed_mps = (circumference_m * delta_pulse) / (_ppr * delta_time);

        // Atualiza variáveis globais (opcional)
        _speed_rps = (delta_pulse / (float)_ppr) / delta_time; // Mantém RPM
        _speed_mps = PI * (_diameter / 1000.0) * _speed_rps;

        // Exemplo: mostra velocidade em m/s
        LOG_INFO("Velocidade: %f m/s", _speed_mps);
    }

    // Atualiza para a próxima medição
    last_pulse = current_pulse;
    last_time = current_time;
    if (_speed_pulse >= 1e6)
    {
        last_pulse -= 1e6; // Ajusta a referência
        _speed_pulse -= 1e6;
    }
}

void MotorControl::motInt()
{
    _speed_pulse = _speed_pulse + 1;
}