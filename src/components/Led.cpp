#include "Led.h"

Led::Led(MQTTManager &mqtt, const String &deviceId, uint8_t pin)
    : _mqtt(mqtt), _pin(pin), _uniqueId("led_Gpio" + String(pin)), _deviceId(deviceId)
{
    pinMode(_pin, OUTPUT);
    _stateTopic = generateTopic(_deviceId, "switch", _uniqueId, "state"); // Armazena o tópico
}

void Led::begin()
{

    ComponentConfig config = {
        .name = "LED GPIO " + String(_pin),
        .unique_id = _uniqueId,
        .type = ComponentType::SWITCH,
        .gpio = _pin,
        .command_topic = generateTopic(_deviceId, "switch", _uniqueId, "command"),
        .state_topic = _stateTopic, // Usa a variável membro
        .callback = &Led::staticHandleCallback,
        .context = this};

    _mqtt.addComponent(config);

    // Publica estado inicial
    bool initialState = digitalRead(_pin);
    _mqtt.publishMessage(_stateTopic.c_str(), initialState ? "ON" : "OFF");
}

void Led::handleStateChange(bool state)
{
    // 1. Atualiza o estado físico
    digitalWrite(_pin, state);
    Serial.printf("LED %d alterado para %s\n", _pin, state ? "ON" : "OFF");

    // 2. Retorna o novo estado para o Home Assistant
    _mqtt.publishMessage(_stateTopic.c_str(), state ? "ON" : "OFF");

    // 3. (Opcional) Feedback adicional
    Serial.printf("Estado publicado no tópico: %s\n", _stateTopic.c_str());
}

void Led::staticHandleCallback(bool state, void *context)
{
    if (context != nullptr)
    {
        static_cast<Led *>(context)->handleStateChange(state);
    }
}