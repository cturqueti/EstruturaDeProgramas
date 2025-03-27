#include <Arduino.h>
#include "utils.h"

LogLevel Utils::currentLogLevel = LogLevel::DEBUG_ALL;
bool Utils::colorsEnabled = true;

const String Utils::getColorCode(LogLevel level)
{
    if (!colorsEnabled)
        return "";

    switch (level)
    {
    case LogLevel::DEBUG_ALL:
        return "\033[0;32m"; // Verde
    case LogLevel::INFO_AND_ABOVE:
        return "\033[0;36m"; // Ciano
    case LogLevel::WARNING_AND_ABOVE:
        return "\033[0;33m"; // Amarelo
    case LogLevel::ERROR_ONLY:
        return "\033[0;31m"; // Vermelho
    default:
        return "\033[0m"; // Reset
    }
}

void Utils::customDelay(unsigned long ms)
{
    unsigned long start = millis();
    while (millis() - start < ms)
    {
        // Espera
    }
}

void Utils::enableLogColors(bool enable)
{
    colorsEnabled = enable;
}

void Utils::setLogLevel(LogLevel level)
{
    currentLogLevel = level;
}

void Utils::log(LogLevel level, const String &tag, const char *format, ...)
{
    // Verifica se o log deve ser exibido
    if (level < currentLogLevel)
    {
        return;
    }

    // Formata a mensagem
    char message[256];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    String colorCode = getColorCode(level);
    String resetCode = colorsEnabled ? "\033[0m" : "";

    // Imprime com cor (opcional) e formato padrão
    Serial.printf("%s [%s][%lu] %s %s\n",
                  colorCode.c_str(),
                  tag.c_str(),
                  millis(),
                  message,
                  resetCode.c_str());
}

String generateTopic(const String &device_id, const String &component_type,
                     const String &unique_id, const String &suffix)
{
    return "homeassistant/" + component_type + "/" + device_id + "/" + unique_id + "/" + suffix;
}

String generateTopic(const String &device_id, const String &suffix)
{
    return "homeassistant/" + device_id + "/" + suffix;
}

void saveCredentialsToNVS()
{
    if (strlen(WIFI_SSID) > 0 && strlen(WIFI_PASSWORD) > 0)
    {
        Preferences preferences;
        preferences.begin("wifi-creds", false);

        preferences.putString("ssid", WIFI_SSID);
        preferences.putString("password", WIFI_PASSWORD);

        preferences.end();
        LOG_INFO("Credenciais WIFI gravadas na NVS com sucesso!");
    }
    else
    {
        LOG_INFO("Erro: Credenciais WIFI não definidas no secrets.env");
    }

    if (strlen(MQTT_USER) > 0 && strlen(MQTT_PASSWORD) > 0)
    {
        Preferences preferences;
        preferences.begin("mqtt-creds", false);

        preferences.putString("user", MQTT_USER);
        preferences.putString("password", MQTT_PASSWORD);

        preferences.end();
        LOG_INFO("Credenciais MQTT gravadas na NVS com sucesso!");
    }
    else
    {
        LOG_INFO("Erro: Credenciais MQTT não definidas no secrets.env");
    }

    if (strlen(OTA_PASSWORD) > 0)
    {
        Preferences preferences;
        preferences.begin("ota-creds", false);

        preferences.putString("password", OTA_PASSWORD);

        preferences.end();
        LOG_INFO("Credenciais OTA gravadas na NVS com sucesso!");
    }
    else
    {
        LOG_INFO("Erro: Credenciais OTA não definidas no secrets.env");
    }
}