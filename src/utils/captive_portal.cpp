#include "captive_portal.h"
#include "utils.h"
#include "components/Led.h"

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
    preferences.begin("wifi-config", true);
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    String mqttUser = preferences.getString("mqtt_user", "");
    String mqttPass = preferences.getString("mqtt_pass", "");
    preferences.end();

    if (ssid.isEmpty() || password.isEmpty() || mqttUser.isEmpty() || mqttPass.isEmpty())
    {
        shouldStartPortal = true;
        LOG_WARN("Credenciais incompletas, iniciando portal");
    }
}

void handleRoot()
{
    LOG_DEBUG("Requisição recebida na raiz");
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
    String mqttPass = server.arg("mqtt_pass");

    Preferences preferences;
    preferences.begin("wifi-config", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.putString("mqtt_user", mqttUser);
    preferences.putString("mqtt_pass", mqttPass);
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