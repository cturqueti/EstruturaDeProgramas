#ifndef LED_h
#define LED_h

#pragma once
#include "mqtt_manager.h"
#include "utils.h"
#include "config.h"

class Led
{
public:
    Led(MQTTManager &mqtt, const String &deviceId, uint8_t pin);
    void begin();

private:
    MQTTManager &_mqtt;
    uint8_t _pin;
    String _uniqueId;
    String _stateTopic;
    String _deviceId;

    // Método de instância para tratar o callback
    void handleStateChange(bool state);

    // Método estático wrapper
    static void staticHandleCallback(bool state, void *context);
};

#endif // LED_h