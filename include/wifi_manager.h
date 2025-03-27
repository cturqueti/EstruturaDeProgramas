// ---------------------------------------------------------------------------------------------------------------- //
//
// Descrição: Este arquivo define a interface do gerenciador de Wi-Fi, incluindo protótipos de funções para inicialização e manutenção da conexão Wi-Fi.
//
// O que pode ser adicionar:
// Protótipos de funções para inicialização e manutenção do Wi-Fi.
// Definições de estruturas de dados, se necessário (por exemplo, para armazenar informações de conexão).
//
// ---------------------------------------------------------------------------------------------------------------- //

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "wifi_config.h"
#include "secrets.h"
#include <IPAddress.h>
#include "utils.h"
#include "logging_macros.h"

#ifdef ESP32
#include <ESPmDNS.h>
#else
#include <ESP8266mDNS.h>
#endif

class WiFiManager
{
public:
    WiFiManager();
    ~WiFiManager();

    void initWiFi();
    void handleWiFi();

    inline bool isConnected() { return WiFi.isConnected(); }

    bool reconnectWIFI();

private:
    bool _wifiTaskActive = false;
};

#endif // WIFI_MANAGER_H