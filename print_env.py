Import("env")
import os

def print_env_vars():
    print("Variáveis carregadas:")
    print(f"WIFI_SSID: {os.getenv('WIFI_SSID')}")
    print(f"WIFI_PASS: {os.getenv('WIFI_PASS')}")
    print(f"MQTT_USER: {os.getenv('MQTT_USER')}")
    print(f"MQTT_PASSWORD: {os.getenv('MQTT_PASSWORD')}")
    print(f"OTA_PASSWORD: {os.getenv('OTA_PASSWORD')}")

# Adicione esta linha no final
print_env_vars()