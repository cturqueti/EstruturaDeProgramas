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
      LOG_DEBUG("Reconectado ao Wi-Fi!");
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
    LOG_DEBUG("Conectando ao Wi-Fi...");
    if (!_wifiTaskActive)
    {
      BaseType_t result = xTaskCreate(connectWiFiStatic, "WiFi Connect", 8192, this, 2, NULL);
      LOG_DEBUG("Tarefa de WiFi criada...");
      _wifiTaskActive = true;
    }
  }
}

bool WiFiManager::reconnectWIFI()
{
  Preferences preferences;
  preferences.begin("wifi-creds", true);

  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");

  preferences.end();

  if (ssid == "" || password == "")
  {
    LOG_ERROR("Credenciais não encontradas na NVS");
    return false;
  }

  WiFi.begin(ssid.c_str(), password.c_str());

  int retries = 0;

  while (WiFi.status() != WL_CONNECTED && retries < 10)
  {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    LOG_DEBUG("Conectando ao Wi-Fi...");
    delay(1000);
    switch (WiFi.status())
    {
    case WL_NO_SSID_AVAIL:
      LOG_ERROR("SSID indisponível.");
      break;

    case WL_CONNECT_FAILED:
      LOG_ERROR("Falha na conexão.");
      break;

    case WL_CONNECTION_LOST:
      LOG_ERROR("Conexão perdida.");
      break;

    case WL_DISCONNECTED:
      LOG_WARN("Desconectado.");
      break;

    case WL_CONNECTED:
      LOG_INFO("Conectado ao Wi-Fi.");
      break;

    default:
      LOG_WARN("Desconhecido: %d", WiFi.status());
      break;
    }
  }
  _wifiTaskActive = false;

  if (WiFi.status() == WL_CONNECTED)
  {
    LOG_DEBUG("Conectado ao Wi-Fi!");
    LOG_INFO("IP: %s", WiFi.localIP().toString().c_str());

    // Inicialização do mDNS
    LOG_DEBUG("Iniciando o mDNS: ");
    MDNS.begin(HOSTNAME);
    LOG_INFO("%s.local", HOSTNAME);
    return true;
  }
  else
  {
    return false;
  }
}
