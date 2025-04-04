#ifndef OTA_H
#define OTA_H

#include "Preferences.h"
#include "utils/utils.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <LogLibrary.h>

class Ota
{
public:
    static bool begin();

    inline bool isOtaActive() const { return _otaActive; }

private:
    bool _otaActive = false;

    void handleOta();
};

void connectOta(void *pvParameters);

#endif // OTA_H