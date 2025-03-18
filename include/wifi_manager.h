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

class WiFiManager {
public:
    WiFiManager();
    ~WiFiManager();

    void initWiFi();
    void handleWiFi();

private:
    // Variáveis membro
};

#endif // WIFI_MANAGER_H