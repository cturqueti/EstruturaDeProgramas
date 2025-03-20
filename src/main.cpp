#include <WiFi.h>
#include <PubSubClient.h>

// Configurações da rede Wi-Fi
const char *ssid = "SIDEOUT 2";        // Nome da sua rede Wi-Fi
const char *password = "sideout_2301"; // Senha da sua rede Wi-Fi

// Configurações do servidor MQTT
const char *mqtt_server = "192.168.0.183";     // Endereço IP do servidor MQTT
const int mqtt_port = 1883;                    // Porta do servidor MQTT
const char *mqtt_user = "sideoutuser";         // Usuário MQTT
const char *mqtt_password = "sideoutpassword"; // Senha MQTT

// Tópico MQTT
const char *mqtt_topic = "test/topic";

// Objetos WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Função para conectar ao Wi-Fi
void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("Conectando à rede: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi conectado");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
}

// Função para reconectar ao servidor MQTT
void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Tentando conexão MQTT...");
        if (client.connect("ESP32Client", mqtt_user, mqtt_password))
        {
            Serial.println("Conectado ao servidor MQTT!");
        }
        else
        {
            Serial.print("Falha, rc=");
            Serial.print(client.state());
            Serial.println(" Tentando novamente em 5 segundos...");
            delay(5000);
        }
    }
}

// Setup
void setup()
{
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
}

// Loop
void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    // Publica uma mensagem no tópico "test/topic" a cada 5 segundos
    static unsigned long lastMsg = 0;
    unsigned long now = millis();
    if (now - lastMsg > 5000)
    {
        lastMsg = now;

        // Mensagem a ser publicada
        String msg = "Hello, MQTT from ESP32!";
        Serial.print("Publicando mensagem: ");
        Serial.println(msg);

        // Publica a mensagem
        client.publish(mqtt_topic, msg.c_str());
    }
}