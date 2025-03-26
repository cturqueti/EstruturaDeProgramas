#ifndef WIFI_POWER_H
#define WIFI_POWER_H

#pragma once
#include <WiFi.h>
#include "mqtt_manager.h"
#include "utils.h"

class WifiPower
{
public:
    WifiPower(MQTTManager &mqtt, const String &deviceId);
    void begin();
    void update();

private:
    MQTTManager &_mqtt;
    String _uniqueId;
    String _stateTopic;
    String _deviceId;
};
#endif // WIFI_POWER_H