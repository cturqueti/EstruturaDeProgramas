#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#pragma once
#include "mqtt_manager.h"
#include "utils.h"

class TemperatureSensor
{
public:
    TemperatureSensor(MQTTManager &mqtt, const String &deviceId, uint8_t analogPin, bool testMode = false);
    void begin();
    void update();
    float readTemperature(); // Método público para leitura

private:
    MQTTManager &_mqtt;
    uint8_t _analogPin;
    String _uniqueId;
    String _stateTopic;
    String _deviceId;
    bool _testMode;
    float _testValue = 25.5f; // Valor fixo para testes

    void publishData(float temperature);
    static float analogToTemperature(int analogValue);
};

#endif // TEMPERATURE_SENSOR_H