#include "ota.h"

/**
 * Inicializa o módulo de atualiza  o OTA.
 *
 * Este método busca a senha OTA armazenada na NVS e configura o
 * módulo de atualiza  o OTA com a senha encontrada.
 *
 * Se a senha OTA não for encontrada, o método retorna false.
 *
 * @return true se a inicializa  o for bem-sucedida, false caso contrário.
 */
bool Ota::begin()
{
    Preferences preferences;
    preferences.begin("ota-creds", true);

    String password = preferences.getString("ota_password", "");

    preferences.end();
    LOG_INFO("password: %s", password.c_str());

    if (password.isEmpty())
    {
        LOG_ERROR("Credenciais OTA não encontradas na NVS");
        return false;
    }

    ArduinoOTA.setHostname("ESP32-Dinamico");
    ArduinoOTA.setPassword(password.c_str());
    ArduinoOTA.onStart([]()
                       { LOG_INFO("Início da atualização OTA"); });

    ArduinoOTA.onError([](ota_error_t error)
                       { LOG_ERROR("Erro OTA [%u]: ", error); });

    ArduinoOTA.begin();

    return true;
}

/**
 * Inicia a tarefa de atualização OTA.
 *
 * Este metodo cria uma tarefa que inicializa o módulo de atualização OTA,
 * se as credenciais estiverem armazenadas na NVS.
 *
 * Se a inicialização for bem-sucedida, a variavel _otaActive ser setada
 * para true.
 */
void Ota::handleOta()
{
    LOG_DEBUG("Iniciando task do OTA");
    BaseType_t result = xTaskCreate(connectOta, "OTA Connect", 1024, this, 2, NULL);
    LOG_DEBUG("Tarefa de OTA criada...");
    _otaActive = true;
}

/**
 * Tarefa responsável por inicializar o módulo de atualização OTA.
 *
 * Caso a inicialização seja bem-sucedida, a tarefa entra em loop,
 * chamando a função handle() do ArduinoOTA periodicamente.
 *
 * Se a inicialização falhar, a tarefa é deletada.
 *
 * @param pvParameters Ponteiro para o objeto Ota que criou esta tarefa.
 */
void connectOta(void *pvParameters)
{
    Ota *instance = static_cast<Ota *>(pvParameters);
    if (instance->begin())
    {
        while (instance->isOtaActive())
        {
            ArduinoOTA.handle();
            vTaskDelay(300 / portTICK_PERIOD_MS);
        }
    }
    vTaskDelete(nullptr);
}