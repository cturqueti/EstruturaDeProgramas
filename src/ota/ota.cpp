#include "ota.h"

void setupOTA()
{
    Preferences preferences;
    preferences.begin("ota-creds", true);

    String password = preferences.getString("ota_password", "");

    preferences.end();
    LOG_INFO("password: %s", password.c_str());

    if (password == "")
    {
        LOG_ERROR("Credenciais OTA não encontradas na NVS");
        return;
    }

    ArduinoOTA.setHostname("ESP32-Dinamico");
    ArduinoOTA.setPassword(password.c_str());
    LOG_DEBUG("Iniciando OTA...");
    ArduinoOTA.begin();
}