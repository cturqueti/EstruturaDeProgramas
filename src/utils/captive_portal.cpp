#include "captive_portal.h"
#include "components/Led.h"
#include "utils.h"
#include "utils/captive_portal_html.h"

WebServer server(80);

CaptivePortal::CaptivePortal()
{
    //_shouldStartPortal = false;
    _portalStartTime = 0;
}

CaptivePortal::~CaptivePortal() {}

bool CaptivePortal::startCaptivePortal()
{
    if (_shouldStartPortal)
    {
        LOG_WARN("Portal já está ativo");
        return true;
    }

    _portalStartTime = millis();

    if (wifiManager.beginAP())
    {
        _shouldStartPortal = true;
        dnsServer.start(53, "*", WiFi.softAPIP());

        server.on("/", HTTP_GET, [this]()
                  { this->handleRoot(); });
        server.on("/scan", HTTP_GET, [this]()
                  { this->handleScan(); });
        server.on("/save", HTTP_POST, [this]()
                  { this->handleSave(); });
        server.on("/getNetworks", HTTP_GET, [this]()
                  { this->handleGetNetworks(); });

        server.onNotFound([]()
                          {
            server.sendHeader("Location", "http://" + WiFi.softAPIP().toString(), true);
            server.send(302, "text/plain", ""); });

        server.begin();
        LOG_DEBUG("Servidor DNS e HTTP iniciados");
        if (!startTask())
        {
            LOG_ERROR("Falha ao iniciar task do portal");
            stopCaptivePortal();
            return false;
        }
        else
        {
            LOG_INFO("Portal cativo iniciado com sucesso");
            return true;
        }
    }
    else
    {
        LOG_ERROR("Falha ao iniciar AP WiFi");
        return false;
    }
    return false;
}

void CaptivePortal::stopCaptivePortal()
{
    _shouldStartPortal = false;
    server.stop();
    dnsServer.stop();
    WiFi.softAPdisconnect(true);
    LOG_DEBUG("Portal parado");
}

void CaptivePortal::handleRoot()
{
    String html;
    LOG_DEBUG("Requisição recebida na raiz");

    String networks = scanNetworks();

    File file = LittleFS.open("/config.html", "r");
    if (file)
    {
        html = file.readString(); // Lê todo o conteúdo
        file.close();             // Fecha o arquivo!
    }
    else
    {
        // Fallback: HTML embutido na flash (use PROGMEM se possível)
        html = String(config_html);
        LOG_DEBUG("Falha ao abrir /config.html, usando HTML embutido");
    }

    html.replace("<!-- NETWORKS_PLACEHOLDER -->", networks);

    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.send(200, "text/html", html); // Envie o HTML modificado
}

void CaptivePortal::handleSave()
{
    if (!server.hasArg("ssid") || !server.hasArg("mqtt_user"))
    {
        server.send(400, "text/html", "<h1>Erro</h1><p>SSID e usuário MQTT são obrigatórios!</p>");
        return;
    }

    String ssid = server.arg("ssid");
    String password = server.arg("password");
    String mqttUser = server.arg("mqtt_user");
    String mqttPassword = server.arg("mqtt_password");
    String otaPassword = server.arg("ota_password");

    LOG_INFO("Verificando credenciais WiFi...");
    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long startTime = millis();
    bool wifiConnected = false;

    // String waitResponse = R"(
    //     <html><body style='font-family: Arial; text-align: center;'>
    //     <h1 style='color: orange;'>Tentando estabelecer a Conexão WiFi!</h1>
    //     <p>Tentando estabelecer a conexão WiFi com as credenciais fornecidas.</p>
    //     <p>Por favor aguarde.</p>
    //     </body></html>
    //     )";
    // server.send(200, "text/html", waitResponse);

    // Tentar conectar por até 10 segundos
    while (millis() - startTime < 10000)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            wifiConnected = true;
            break;
        }
        delay(500);
        LOG_DEBUG("Aguardando conexão WiFi...");
    }

    if (!wifiConnected)
    {
        String errorResponse = R"(
        <html><body style='font-family: Arial; text-align: center;'>
        <h1 style='color: red;'>Falha na Conexão WiFi!</h1>
        <p>Não foi possível conectar ao WiFi com as credenciais fornecidas.</p>
        <p>Por favor, <a href='/'>volte</a> e verifique seu SSID e senha.</p>
        </body></html>
        )";
        server.send(200, "text/html", errorResponse);
        WiFi.disconnect();
        return;
    }

    LOG_INFO("WiFi conectado com sucesso!");

    Preferences preferences;
    preferences.begin("wifi-creds", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
    LOG_INFO("Salvado as configurações WIFI");

    preferences.begin("mqtt-creds", false);
    preferences.putString("mqtt_user", mqttUser);
    preferences.putString("mqtt_password", mqttPassword);
    preferences.end();
    LOG_INFO("Salvado as configurações MQTT");

    preferences.begin("ota-creds", false);
    preferences.putString("ota_password", otaPassword);
    preferences.end();
    LOG_INFO("Salvado as configurações OTA");

    String response = R"(
    <html><body style='font-family: Arial; text-align: center;'>
    <h1 style='color: green;'>Configuração Salva!</h1>
    <p>O dispositivo será reiniciado em...</p>
    <script>
        setTimeout(function(){ window.location.href = '/'; }, 2000);
    </script>
    </body></html>
    )";

    server.send(200, "text/html", response);
    // delay(5000);
    stopCaptivePortal();
    ESP.restart();
}

void CaptivePortal::handlePortal()
{
    if (isPortalActive())
    {
        dnsServer.processNextRequest();
        server.handleClient();
    }
    else if (!isCredentials())
    {
        LOG_ERROR("Timeout do portal captivo");
        ESP.restart();
    }
}

void CaptivePortal::handleScan()
{
    LOG_DEBUG("Requisição de escaneamento recebida");
    String networks = scanNetworks();
    server.send(200, "text/html", networks);
}

String CaptivePortal::scanNetworksToJSON()
{
    JsonDocument doc;
    JsonArray networks = doc.to<JsonArray>();

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
        JsonObject network = networks.add<JsonObject>();
        network["ssid"] = WiFi.SSID(i);
        network["rssi"] = WiFi.RSSI(i);
        network["encrypted"] = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
    }
    LOG_INFO("Escaneando rede");

    String json;
    serializeJson(doc, json);
    return json;
}
void CaptivePortal::handleGetNetworks()
{
    String json = scanNetworksToJSON();
    server.send(200, "application/json", json);
}

String CaptivePortal::scanNetworks()
{
    LOG_INFO("Escaneando redes WiFi...");
    int numNetworks = WiFi.scanNetworks();
    LOG_INFO("Redes encontradas: %d", numNetworks);

    // implementar futuramente um while enquanto não encontrar nenhuma fica varrendo, maximo 10 tentativas

    String options = "";
    for (int i = 0; i < numNetworks; ++i)
    {
        options += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + "dBm)";
        if (WiFi.encryptionType(i) != WIFI_AUTH_OPEN)
        {
            options += " 🔒";
        }
        options += "</option>";
    }
    WiFi.scanDelete(); // Limpa a lista de redes
    return options;
}

String scanNetworks()
{
    LOG_INFO("Escaneando redes WiFi...");
    int numNetworks = WiFi.scanNetworks();
    LOG_INFO("Redes encontradas: %d", numNetworks);

    String options = "";
    for (int i = 0; i < numNetworks; ++i)
    {
        options += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + "dBm)";
        if (WiFi.encryptionType(i) != WIFI_AUTH_OPEN)
        {
            options += " 🔒";
        }
        options += "</option>";
    }
    WiFi.scanDelete(); // Limpa a lista de redes
    return options;
}

bool CaptivePortal::startTask()
{
    LOG_DEBUG("Iniciando tarefa de portal captivo...");
    BaseType_t result = xTaskCreate(connectCaptivePortalStatic, "Captive Portal", 4096, this, 2, NULL);

    if (result != pdPASS)
    {
        LOG_ERROR("Falha ao criar task do portal");
        return false;
    }

    LOG_DEBUG("Tarefa do portal criada com sucesso");
    return true;
}

void connectCaptivePortalStatic(void *pvParameters)
{
    CaptivePortal *instance = static_cast<CaptivePortal *>(pvParameters);
    while (instance->isPortalActive())
    {
        instance->handlePortal();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(nullptr);
}