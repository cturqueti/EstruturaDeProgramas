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
#include "wifi_config.h"
#include <WiFi.h>

void initWiFi() {
    // Inicialização do Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}

void handleWiFi() {
    // Lógica para manter a conexão Wi-Fi
    if (WiFi.status() != WL_CONNECTED) {
        initWiFi();
    }
}