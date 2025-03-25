#include <Arduino.h>
#include "utils.h"

void Utils::customDelay(unsigned long ms)
{
    unsigned long start = millis();
    while (millis() - start < ms)
    {
        // Espera
    }
}

String generateTopic(const String &device_id, const String &component_type,
                     const String &unique_id, const String &suffix)
{
    return "homeassistant/" + device_id + "/" + component_type + "/" + unique_id + "/" + suffix;
}

String generateTopic(const String &device_id, const String &suffix)
{
    return "homeassistant/" + device_id + "/" + suffix;
}