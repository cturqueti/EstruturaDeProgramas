#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(MQTTManager &mqtt, const String &deviceId, uint8_t analogPin, bool testMode)
    : _mqtt(mqtt), _analogPin(analogPin), _sensorId("temp_" + deviceId), _testMode(testMode)
{
    if (!_testMode)
    {
        pinMode(_analogPin, INPUT);
    }
}

void TemperatureSensor::begin()
{
    ComponentConfig config = {
        .name = "Sensor de Temperatura",
        .unique_id = _sensorId,
        .type = ComponentType::SENSOR,
        .state_topic = generateTopic(DEVICE_ID, "sensor", _sensorId, "state"),
        .unit_of_measurement = "°C",
        .device_class = "temperature"};
    _mqtt.addComponent(config);
}

void TemperatureSensor::update()
{
    float temperature = readTemperature();
    publishData(temperature);
}

float TemperatureSensor::readTemperature()
{
    if (_testMode)
    {
        return _testValue; // Retorna valor fixo para testes
    }

    // Leitura real do sensor (exemplo com LM35)
    int analogValue = analogRead(_analogPin);
    return analogToTemperature(analogValue);
}

void TemperatureSensor::publishData(float temperature)
{
    _mqtt.publishSensorData(_sensorId, temperature);
    Serial.printf("Temperatura publicada: %.2f°C\n", temperature);
}

float TemperatureSensor::analogToTemperature(int analogValue)
{
    // Conversão para LM35 (10mV por grau Celsius)
    float voltage = analogValue * (3.3 / 4095.0); // ESP32 tem ADC de 12 bits
    return voltage * 100.0;                       // LM35: 10mV/°C

    // Ajuste esta função conforme seu sensor específico
}