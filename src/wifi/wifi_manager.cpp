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
  handleWiFi();
}

void connectWiFiStatic(void *parameter)
{
  WiFiManager *instance = static_cast<WiFiManager *>(parameter);
  instance->reconnectWIFI();
  while (true)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    if (!instance->isConnected())
    {
      instance->reconnectWIFI();
      Serial.println("Reconectado ao Wi-Fi!");
    }
  }
  vTaskDelete(nullptr);
}

void WiFiManager::handleWiFi()
{
  // Manutenção do Wi-Fi
#ifndef ESP32
  MDNS.update();
#endif

  if (!WiFi.isConnected())
  {
    Serial.println("Conectando ao Wi-Fi...");
    if (!_wifiTaskActive)
    {
      BaseType_t result = xTaskCreate(connectWiFiStatic, "WiFi Connect", 8192, this, 2, NULL);
      Serial.println("Tarefa de WiFi criada...");
      _wifiTaskActive = true;
    }
  }
}

bool WiFiManager::reconnectWIFI()
{

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int retries = 0;

  while (WiFi.status() != WL_CONNECTED && retries < 10)
  {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Conectando ao Wi-Fi...");
    delay(1000);
    switch (WiFi.status())
    {
    case WL_NO_SSID_AVAIL:
      Serial.println("SSID não disponível");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Falha ao conectar");
      break;
    case WL_CONNECTION_LOST:
      Serial.println("Conexão perdida");
      break;
    case WL_DISCONNECTED:
      Serial.println("Desconectado");
      break;
    default:
      Serial.println("Erro desconhecido");
      break;
    }
  }
  _wifiTaskActive = false;

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Conectado ao Wi-Fi!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    // Inicialização do mDNS
    Serial.print("Iniciando o mDNS: ");
    MDNS.begin(HOSTNAME);
    Serial.printf("%s.local\n", HOSTNAME);
    return true;
  }
  else
  {
    return false;
  }
}
