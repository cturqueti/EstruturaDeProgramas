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

#include "config.h"
#include "pinout.h"
#include "utils/utils.h"
#include "wifi_config.h"
#include <Arduino.h>
#include <IPAddress.h>
#include <LogLibrary.h>
#include <WiFi.h>

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

    bool beginClient();
    bool beginAP();
    bool connectWIFI();

    inline bool isConnected() { return WiFi.isConnected(); }

protected:
    bool _wifiTaskActive = false;
    bool _wifiStarted = false;
    String _ssid, _password;
    String _apSSID = "ESP32_portal", _apPassword = "portal1234";

private:
    bool getPreferences();
};

#endif // WIFI_MANAGER_H