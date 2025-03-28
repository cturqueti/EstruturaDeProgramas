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

bool WiFiManager::beginClient()
{
  WiFi.mode(WIFI_STA); // Configura como station
  WiFi.setAutoReconnect(true);

  connectWIFI();

  if (WiFi.isConnected())
  {
    _wifiStarted = true;

    return true;
  }
  else
  {
    return false;
  }
}

bool WiFiManager::beginAP()
{
  WiFi.setAutoReconnect(false);
  WiFi.mode(WIFI_AP); // Configura como AP
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(_apSSID, _apPassword);
  LOG_INFO("AP criado. SSID: %s, IP: %s", _apSSID, WiFi.softAPIP().toString().c_str());
  return true;
}

bool WiFiManager::connectWIFI()
{
  const uint8_t max_attempts = 5;
  int attempts = 0;

  if (getPreferences())
  {
    WiFi.begin(_ssid.c_str(), _password.c_str());
  }
  else
  {
    return false;
  }

  while (WiFi.status() != WL_CONNECTED && attempts < max_attempts)
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

bool WiFiManager::getPreferences()
{
  Preferences preferences;
  preferences.begin("wifi-creds", true);

  _ssid = preferences.getString("ssid", "");
  _password = preferences.getString("password", "");

  preferences.end();

  if (_ssid == "" || _password == "")
  {
    LOG_ERROR("Credenciais não encontradas na NVS");
    // LOG_INFO("ssid: %s\tpassword: %s", ssid.c_str(), password.c_str());
    return false;
  }
  return true;
}
