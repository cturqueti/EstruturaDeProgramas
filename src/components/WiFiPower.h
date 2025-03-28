#ifndef WIFI_POWER_H
#define WIFI_POWER_H

#pragma once
#include "mqtt/mqtt_manager.h"
#include "utils/utils.h"
#include <WiFi.h>

class WifiPower
{
public:
    WifiPower(MQTTManager &mqtt);
    void begin();
    void update();

private:
    MQTTManager &_mqtt;
    String _uniqueId;
    String _stateTopic;
};
#endif // WIFI_POWER_H