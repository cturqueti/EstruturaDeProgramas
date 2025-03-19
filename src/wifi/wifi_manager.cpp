// ---------------------------------------------------------------------------------------------------------------- //
//
// Descrição: Este arquivo contém a implementação do gerenciamento de Wi-Fi.
//
// O que pode ser adicionar:
// Inicialização do Wi-Fi.
// Funções para manter a conexão Wi-Fi.
//
// ---------------------------------------------------------------------------------------------------------------- //

#include "wifi_manager.h"
#include "Arduino.h"
#include "config.h"
#include "pinout.h"
#include <WiFi.h>

WiFiManager::WiFiManager()
{
  // Defina o IP estático

#ifdef WIFI_MODE_STATIC
  // Configure o Wi-Fi com o IP estático
  IPAddress ip(WIFI_STATIC_IP);
  IPAddress gateway(WIFI_STATIC_GATEWAY);
  IPAddress subnet(WIFI_STATIC_SUBNET);
  IPAddress dns(WIFI_STATIC_DNS);
  WiFi.config(ip, gateway, subnet, dns);
#endif
}

WiFiManager::~WiFiManager()
{
  // Destrutor
}

void WiFiManager::initWiFi()
{
  // Inicialização do Wi-Fi

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void WiFiManager::handleWiFi()
{
  // Manutenção do Wi-Fi
}