#ifndef CAPTIVE_PORTAL_HTML_H
#define CAPTIVE_PORTAL_HTML_H

#include <Arduino.h>

// const char config_html[] PROGMEM = R"rawliteral(
const char *config_html = R"=====(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configuração ESP32</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #f5f5f5;
            margin: 0;
            padding: 20px;
            color: #333;
        }
        .container {
            max-width: 500px;
            margin: 0 auto;
            background-color: #fff;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
        }
        h2 {
            color: #2c3e50;
            text-align: center;
            margin-bottom: 25px;
        }
        .form-group {
            margin-bottom: 20px;
        }
        label {
            display: block;
            margin-bottom: 8px;
            font-weight: 600;
        }
        select, input {
            width: 100%;
            padding: 12px;
            border: 1px solid #ddd;
            border-radius: 5px;
            font-size: 16px;
            box-sizing: border-box;
            margin-bottom: 10px;
        }
        select {
            height: 46px;
            appearance: none;
            background-image: url("data:image/svg+xml;charset=UTF-8,%3csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2' stroke-linecap='round' stroke-linejoin='round'%3e%3cpolyline points='6 9 12 15 18 9'%3e%3c/polyline%3e%3c/svg%3e");
            background-repeat: no-repeat;
            background-position: right 10px center;
            background-size: 20px;
        }
        .wifi-strength {
            font-size: 12px;
            color: #666;
            margin-left: 5px;
        }
        .btn {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 12px 20px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
            width: 100%;
            transition: background-color 0.3s;
        }
        .btn:hover {
            background-color: #2980b9;
        }
        .refresh-btn {
            background-color: #f39c12;
            margin-left: 10px;
            padding: 10px 15px;
            width: auto;
            display: inline-flex;
            align-items: center;
        }
        .refresh-btn svg {
            margin-right: 5px;
        }
        .network-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        .lock-icon {
            margin-left: 5px;
            color: #666;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>Configuração do Dispositivo</h2>
        <form action="/save" method="post">
            <div class="form-group">
                <div class="network-header">
                    <label for="ssid">Rede WiFi:</label>
                    <button type="button" onclick="refreshNetworks()" class="btn refresh-btn">
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                            <path d="M23 4v6h-6"></path>
                            <path d="M1 20v-6h6"></path>
                            <path d="M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15"></path>
                        </svg>
                        Atualizar
                    </button>
                </div>
                <select id="ssid" name="ssid" required class="form-control">
                    <option value="">Selecione uma rede WiFi</option>
                    <!-- NETWORKS_PLACEHOLDER -->
                </select>
            </div>

            <div class="form-group">
                <label for="password">Senha WiFi:</label>
                <input type="password" id="password" name="password" placeholder="Digite a senha da rede WiFi">
            </div>

            <div class="form-group">
                <label for="mqtt_user">Usuário MQTT:</label>
                <input type="text" id="mqtt_user" name="mqtt_user" required placeholder="Digite o usuário MQTT">
            </div>

            <div class="form-group">
                <label for="mqtt_password">Senha MQTT:</label>
                <input type="password" id="mqtt_password" name="mqtt_password" placeholder="Digite a senha MQTT">
            </div>

            <div class="form-group">
                <label for="ota_password">Senha OTA (Atualização):</label>
                <input type="password" id="ota_password" name="ota_password" placeholder="Digite a senha para atualizações OTA">
            </div>

            <button type="submit" class="btn">Salvar Configurações</button>
        </form>
    </div>

    <script>
        function refreshNetworks() {
            console.log("Atualizando redes...");
            fetch('/scan')
              .then(response => {
                if (!response.ok) {
                    throw new Error('Erro na rede');
                }
                return response.text();
                })
            .then(html => {
                console.log("Redes recebidas:", html);
                document.getElementById('ssid').innerHTML = 
                    '<option value="">Selecione uma rede WiFi</option>' + html;
            })
            .catch(error => {
                console.error('Erro ao atualizar redes:', error);
                alert('Erro ao atualizar a lista de redes. Tente novamente.');
            });
        }

        // Foca no campo de senha quando uma rede protegida é selecionada
        document.getElementById('ssid').addEventListener('change', function() {
            const selectedOption = this.options[this.selectedIndex];
            if (selectedOption.text.includes('🔒')) {
                document.getElementById('password').focus();
            }
        });
    </script>
</body>
</html>
)=====";

#endif // CAPTIVE_PORTAL_HTML_H