#include "WifiPower.h"

WifiPower::WifiPower(MQTTManager &mqtt, const String &deviceId)
    : _mqtt(mqtt), _uniqueId("wifi_power"), _deviceId(deviceId)
{
    _stateTopic = generateTopic(_deviceId, "sensor", _uniqueId, "state");
}

void WifiPower::begin()
{

    ComponentConfig config = {
        .name = "Potencia WIFI",
        .unique_id = _uniqueId,
        .type = ComponentType::SENSOR,
        .state_topic = _stateTopic,
        .unit_of_measurement = "dBm",
        .device_class = "power"};
    _mqtt.addComponent(config);
}

void WifiPower::update()
{
    int8_t rssi = WiFi.RSSI();
    _mqtt.publishSensorData(_uniqueId, rssi);
}