#ifndef FAN_H
#define FAN_H

#pragma once
#include "mqtt_manager.h"
#include "utils.h"

class Fan
{
public:
    Fan(MQTTManager &mqtt, uint8_t pin);
    void begin();

private:
    MQTTManager &_mqtt;
    uint8_t _pin;
    String _uniqueId;
    String _stateTopic, _commandTopic;
    String _speedStateTopic, _speedCommandTopic;
    int _currentSpeed = 0;
    bool _state = false;

    // Funções de tratamento
    void handlePowerCommand(bool state);
    void handleSpeedCommand(int speed);

    // Callbacks estáticas que redirecionam para a instância
    static void staticPowerCallback(bool state, void *context);
    static void staticSpeedCallback(int speed, void *context);
};

#endif // FAN_H