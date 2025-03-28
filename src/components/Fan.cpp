#include "Fan.h"

Fan::Fan(MQTTManager &mqtt, uint8_t pin)
    : _mqtt(mqtt),
      _pin(pin)
{
    _uniqueId = "fan_" + _mqtt.getDeviceId() + "_" + String(pin);
    pinMode(_pin, OUTPUT);
    _stateTopic = _topic.generate(_mqtt.getDeviceId(), "fan", _uniqueId, "on/state");
    _commandTopic = _topic.generate(_mqtt.getDeviceId(), "fan", _uniqueId, "on/set");
    _speedStateTopic = _topic.generate(_mqtt.getDeviceId(), "fan", _uniqueId, "speed/percentage_state");
    _speedCommandTopic = _topic.generate(_mqtt.getDeviceId(), "fan", _uniqueId, "speed/percentage");
}

void Fan::begin()
{
    ComponentConfig config = {
        .name = "Ventilador Quarto",
        .unique_id = _uniqueId,
        .type = ComponentType::FAN,
        .gpio = _pin,
        ///<.command_topic = generateTopic(_deviceId, "fan", _uniqueId, "power/command"),
        .command_topic = _commandTopic,
        .state_topic = _stateTopic,
        .callback = &Fan::staticPowerCallback,
        .context = this,
        .speed_command_topic = _speedCommandTopic,
        .speed_state_topic = _speedStateTopic,
        .speed_callback = &Fan::staticSpeedCallback};

    _mqtt.addComponent(config);
}

// Implementação dos handlers
void Fan::handlePowerCommand(bool state)
{
    _state = state;
    digitalWrite(_pin, state ? HIGH : LOW);
    LOG_DEBUG("Fan %d: Estado alterado para %s\n", _pin, state ? "ON" : "OFF");

    // Publica estado atual
    _mqtt.publishMessage(_stateTopic.c_str(), state ? "ON" : "OFF");
}

void Fan::handleSpeedCommand(int speed)
{
    speed = constrain(speed, 0, 100);
    _currentSpeed = speed;

    // Mapeia para PWM (0-255)
    int pwmValue = map(speed, 0, 100, 0, 255);
    analogWrite(_pin, pwmValue);

    LOG_DEBUG("Fan %d: Velocidade alterada para %d%%\n", _pin, speed);

    // Publica estado atual
    _mqtt.publishMessage(
        _speedStateTopic.c_str(),
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