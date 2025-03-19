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

#ifdef ESP32
#include <ESPmDNS.h>
#else
#include <ESP8266mDNS.h>
#endif

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

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi!");
  MDNS.begin(HOSTNAME);
}

void WiFiManager::handleWiFi()
{
  // Manutenção do Wi-Fi
#ifndef ESP32
  MDNS.update();
#endif
}
