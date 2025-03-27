// ---------------------------------------------------------------------------------------------------------------- //
//
// Descrição: Este arquivo contém funções utilitárias que podem ser usadas em todo o projeto.
//
// O que pode ser adicionar:
// Funções de delay.
// Funções de conversão (por exemplo, de inteiro para string).
// Funções de manipulação de strings.
//
//  Obs.: A palavra-chave inline diz ao compilador para incluir a implementação da função diretamente no local onde ela é chamada, evitando múltiplas definições.
//  Se a função for grande ou complexa, é melhor movê-la para um arquivo .cpp.
// ---------------------------------------------------------------------------------------------------------------- //

#ifndef UTILS_H
#define UTILS_H

#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

#ifndef MQTT_USER
#define MQTT_USER ""
#endif

#ifndef MQTT_PASSWORD
#define MQTT_PASSWORD ""
#endif

#ifndef OTA_PASSWORD
#define OTA_PASSWORD ""
#endif

#include <config.h>
#include <Preferences.h>
#include "logging_macros.h"

enum class LogLevel : uint8_t
{
    DEBUG_ALL = 0,
    INFO_AND_ABOVE,
    WARNING_AND_ABOVE,
    ERROR_ONLY,
    NONE
};

class Utils
{
public:
    static void customDelay(unsigned long ms);
    static void setLogLevel(LogLevel level);
    static void enableLogColors(bool enable);
    static void log(LogLevel level, const String &tag, const char *format, ...);
    // void printf(const String &tag, const char *format, ...);

private:
    static LogLevel currentLogLevel;
    static bool colorsEnabled;

    static const String getColorCode(LogLevel level);
};

String generateTopic(const String &device_id, const String &component_type,
                     const String &unique_id, const String &suffix);

String generateTopic(const String &device_id, const String &suffix);

void saveCredentialsToNVS();

#endif // UTILS_H
