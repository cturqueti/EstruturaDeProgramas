#include "Fan.h"

Fan::Fan(MQTTManager &mqtt, const String &deviceId, uint8_t pin)
    : _mqtt(mqtt), _pin(pin), _uniqueId("fan_" + deviceId + "_" + String(pin))
{
    pinMode(_pin, OUTPUT);
}

void Fan::begin()
{
    ComponentConfig config = {
        .name = "Ventilador Quarto",
        .unique_id = _uniqueId,
        .type = ComponentType::FAN,
        .gpio = _pin,
        .command_topic = generateTopic(DEVICE_ID, "fan", _uniqueId, "power/command"),
        .state_topic = generateTopic(DEVICE_ID, "fan", _uniqueId, "power/state"),
        .callback = &Fan::staticPowerCallback,
        .context = this,
        .speed_command_topic = generateTopic(DEVICE_ID, "fan", _uniqueId, "speed/command"),
        .speed_state_topic = generateTopic(DEVICE_ID, "fan", _uniqueId, "speed/state"),
        .speed_callback = &Fan::staticSpeedCallback};

    _mqtt.addComponent(config);
}

// Implementação dos handlers
void Fan::handlePowerCommand(bool state)
{
    _state = state;
    digitalWrite(_pin, state ? HIGH : LOW);
    Serial.printf("Fan %d: Estado alterado para %s\n", _pin, state ? "ON" : "OFF");

    // Publica estado atual
    _mqtt.publishMessage(
        generateTopic(DEVICE_ID, "fan", _uniqueId, "power/state").c_str(),
        state ? "ON" : "OFF");
}

void Fan::handleSpeedCommand(int speed)
{
    speed = constrain(speed, 0, 100);
    _currentSpeed = speed;

    // Mapeia para PWM (0-255)
    int pwmValue = map(speed, 0, 100, 0, 255);
    analogWrite(_pin, pwmValue);

    Serial.printf("Fan %d: Velocidade alterada para %d%%\n", _pin, speed);

    // Publica estado atual
    _mqtt.publishMessage(
        generateTopic(DEVICE_ID, "fan", _uniqueId, "speed/state").c_str(),
        String(speed).c_str());
}

// Callbacks estáticas
void Fan::staticPowerCallback(bool state, void *context)
{
    if (context != nullptr)
    {
        static_cast<Fan *>(context)->handlePowerCommand(state);
    }
}

void Fan::staticSpeedCallback(int speed, void *context)
{
    if (context != nullptr)
    {
        static_cast<Fan *>(context)->handleSpeedCommand(speed);
    }
}