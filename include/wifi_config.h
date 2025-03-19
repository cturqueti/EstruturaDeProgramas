// ---------------------------------------------------------------------------------------------------------------- //
//
// Descrição: Este arquivo contém configurações específicas para Wi-Fi.
//
// O que pode ser adicionar:
// Configurações de IP estático, se necessário.
//
// ---------------------------------------------------------------------------------------------------------------- //

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// Defina se o Wi-Fi deve ser estático ou dinâmico
// #define WIFI_MODE_STATIC 1
#define WIFI_MODE_DYNAMIC 0

// Defina o IP estático (se WIFI_MODE_STATIC for definido)
#define WIFI_STATIC_IP "192.168.0.100"
#define WIFI_STATIC_GATEWAY "192.168.0.1"
#define WIFI_STATIC_SUBNET "255.255.255.0"
#define WIFI_STATIC_DNS "8.8.8.8"

#endif // WIFI_CONFIG_H