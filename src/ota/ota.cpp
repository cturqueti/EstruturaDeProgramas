#include "ota.h"

void setupOTA()
{
    Preferences preferences;
    preferences.begin("ota-creds", true);

    String password = preferences.getString("password", "");

    preferences.end();

    if (password == "")
    {
        LOG_ERROR("Credenciais não encontradas na NVS");
        return;
    }

    ArduinoOTA.setHostname("ESP32-Dinamico");
    ArduinoOTA.setPassword(password.c_str());
    LOG_DEBUG("Iniciando OTA...");
    ArduinoOTA.begin();
}