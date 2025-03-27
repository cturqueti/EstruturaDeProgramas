#include "WifiPower.h"

WifiPower::WifiPower(MQTTManager &mqtt)
    : _mqtt(mqtt),
      _uniqueId("wifi_power")
{
    _stateTopic = generateTopic(_mqtt.getDeviceId(), "sensor", _uniqueId, "state");
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