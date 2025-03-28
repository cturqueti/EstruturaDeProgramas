#ifndef LED_h
#define LED_h

#pragma once
#include "mqtt/mqtt_manager.h"
#include "utils/utils.h"

class Led
{
public:
    Led(MQTTManager &mqtt, uint8_t pin);
    void begin();

private:
    MQTTManager &_mqtt;
    uint8_t _pin;
    String _uniqueId;
    String _stateTopic, _commandTopic;

    // Método de instância para tratar o callback
    void handleStateChange(bool state);

    // Método estático wrapper
    static void staticHandleCallback(bool state, void *context);
};

#endif // LED_h