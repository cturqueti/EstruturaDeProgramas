#pragma once
#include <vector>
#include <functional>
#include "Arduino.h"

/**
 * @enum ComponentType
 * @brief Tipos de componentes suportados pelo Home Assistant via MQTT Discovery
 * @note Atualizado para a versão 2023.12 do Home Assistant
 */
enum class ComponentType
{
    /*---------- Componentes Básicos ----------*/
    SWITCH,        ///< Interruptores/Relés (ON-OFF simples)
    SENSOR,        ///< Sensores numéricos (temperatura, umidade, etc.)
    BINARY_SENSOR, ///< Sensores binários (porta aberta/fechada, movimento)
    LIGHT,         ///< Controle de iluminação (dimmer, RGB, efeitos)
    COVER,         ///< Cortinas, persianas e controles motorizados
    CLIMATE,       ///< Termostatos e controle de HVAC
    FAN,           ///< Ventiladores (velocidade variável)
    LOCK,          ///< Fechaduras inteligentes
    VACUUM,        ///< Aspiradores robôs
    MOTOR,         ///< Controles motorizados

    /*---------- Dispositivos de Mídia ----------*/
    CAMERA,       ///< Transmissão de vídeo ou imagens estáticas
    MEDIA_PLAYER, ///< Controle de players de áudio/vídeo

    /*---------- Componentes de Automação ----------*/
    DEVICE_TRACKER,      ///< Rastreamento de dispositivos (presença)
    ALARM_CONTROL_PANEL, ///< Sistemas de alarme
    SCENE,               ///< Cenas pré-configuradas
    SCRIPT,              ///< Sequências automatizadas
    AUTOMATION,          ///< Regras de automação

    /*---------- Componentes Modernos (2022+) ----------*/
    BUTTON,   ///< Botões virtuais para disparar ações
    SELECT,   ///< Seleção entre opções pré-definidas
    NUMBER,   ///< Controle numérico com ajuste preciso
    UPDATE,   ///< Gerenciamento de atualizações de firmware
    TEXT,     ///< Entrada/Saída de texto livre
    DATETIME, ///< Controle de data/hora
    EVENT,    ///< Disparador de eventos customizados

    /*---------- Componentes Especializados ----------*/
    IMAGE,        ///< Exibição de imagens estáticas
    TAG,          ///< Leitura de tags RFID/NFC
    VALVE,        ///< Controle de válvulas (água/gás)
    WATER_HEATER, ///< Aquecedores de água
    HUMIDIFIER,   ///< Controladores de umidade
    LAWN_MOWER,   ///< Robôs cortadores de grama

    /*---------- Componentes de Utilitários ----------*/
    MQTT,     ///< Integração genérica MQTT
    TEMPLATE, ///< Entidades criadas via templates
    TOD,      ///< Temporizadores (Time-of-Day)

    /*---------- Componentes de Sistema ----------*/
    SYSTEM_HEALTH,           ///< Monitoramento do sistema
    PERSISTENT_NOTIFICATION, ///< Notificações persistentes

    /*---------- Componentes de Integração ----------*/
    CALENDAR,   ///< Integração com calendários
    MAILBOX,    ///< Monitoramento de caixa postal
    REMOTE,     ///< Controle remoto infravermelho
    NOTIFY,     ///< Serviços de notificação
    WIFI_SIGNAL ///< Monitoramento de sinal de Wi-Fi
};

struct ComponentConfig
{
    String name;
    String unique_id;
    ComponentType type;
    uint8_t gpio;
    String command_topic;
    String state_topic;
    std::function<void(bool, void *)> callback; // Para switches
    void *context;
    std::function<float()> sensor_callback; // Para sensores
    String unit_of_measurement;
    String device_class; // ← Adicione esta linha (recomendo após unit_of_measurement)

    // Campos específicos para fan
    int pwm_channel;
    int pwm_frequency;
    int pwm_resolution;
    String speed_command_topic;
    String speed_state_topic;
    String speeds;
    String percentage_command_topic; // Pode ser o mesmo que speed_command_topic
    String percentage_state_topic;   // Pode ser o mesmo que speed_state_topic
    int last_speed;
    std::function<void(int, void *)> speed_callback;
    void *speed_context;
};