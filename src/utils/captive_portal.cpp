#include "captive_portal.h"
#include "utils.h"
#include "components/Led.h"
#include "utils/captive_portal_html.h"

WebServer server(80);
DNSServer dnsServer;

const char *apSSID = "ESP32-Config";
const char *apPassword = "config1234";
bool shouldStartPortal = false;
unsigned long portalStartTime = 0;
const unsigned long PORTAL_TIMEOUT = 5 * 60 * 1000; // 5 minutos

void checkCredentials()
{
    Preferences preferences;
    preferences.begin("wifi-creds", true);
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    LOG_INFO("ssid: %s\tpassword: %s", ssid.c_str(), password.c_str());
    preferences.end();

    preferences.begin("mqtt-creds", true);
    String mqttUser = preferences.getString("mqtt_user", "");
    String mqttPassword = preferences.getString("mqtt_password", "");
    LOG_INFO("user: %s\tpassword: %s", mqttUser.c_str(), mqttPassword.c_str());
    preferences.end();

    preferences.begin("ota-creds", true);
    String otaPassword = preferences.getString("ota_password", "");
    LOG_INFO("password: %s", otaPassword.c_str());
    preferences.end();

    if (ssid.isEmpty() || password.isEmpty() || mqttUser.isEmpty() || mqttPassword.isEmpty())
    {
        shouldStartPortal = true;
        LOG_WARN("Credenciais incompletas, iniciando portal");
    }
}

void handleRoot()
{
    LOG_DEBUG("Requisição recebida na raiz");

    String networks = scanNetworks();
    String dynamicHTML = String(config_html);
    dynamicHTML.replace("<!-- NETWORKS_PLACEHOLDER -->", networks);

    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.send(200, "text/html", config_html);
}

void handleSave()
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

    preferences.begin("mqtt-creds", false);
    preferences.putString("mqtt_user", mqttUser);
    preferences.putString("mqtt_password", mqttPassword);
    preferences.end();

    preferences.begin("ota-creds", false);
    preferences.putString("ota_password", otaPassword);
    preferences.end();

    String response = R"(
    <html><body style='font-family: Arial; text-align: center;'>
    <h1 style='color: green;'>Configuração Salva!</h1>
    <p>O dispositivo será reiniciado em 5 segundos...</p>
    <script>
        setTimeout(function(){ window.location.href = '/'; }, 5000);
    </script>
    </body></html>
    )";

    server.send(200, "text/html", response);
    delay(5000);
    ESP.restart();
}

void startConfigPortal()
{
    portalStartTime = millis();
    WiFi.softAP(apSSID, apPassword);
    LOG_INFO("AP criado. SSID: %s, IP: %s", apSSID, WiFi.softAPIP().toString().c_str());

    dnsServer.start(53, "*", WiFi.softAPIP());

    server.on("/", HTTP_GET, handleRoot);
    server.on("/scan", HTTP_GET, handleScan);
    server.on("/save", HTTP_POST, handleSave);
    server.onNotFound([]()
                      {
        server.sendHeader("Location", "http://" + WiFi.softAPIP().toString(), true);
        server.send(302, "text/plain", ""); });

    server.begin();
    LOG_DEBUG("Servidor DNS e HTTP iniciados");
}

bool isPortalActive()
{
    return shouldStartPortal;
    // return shouldStartPortal && (millis() - portalStartTime < PORTAL_TIMEOUT);
}

void handlePortal()
{
    if (isPortalActive())
    {
        // LOG_INFO("Processando requisição do portal");
        dnsServer.processNextRequest();
        server.handleClient();
    }
    else if (shouldStartPortal)
    {
        LOG_ERROR("Timeout do portal captivo");
        ESP.restart();
    }
}

void handleScan()
{
    LOG_DEBUG("Requisição de escaneamento recebida");
    String networks = scanNetworks();
    server.send(200, "text/html", networks);
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