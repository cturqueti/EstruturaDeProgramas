#include <Arduino.h> // Inclui as funções e constantes do Arduino
#include <unity.h>   // Biblioteca para testes unitários no Arduino
#include "modules/module1.h" // Inclui o módulo a ser testado
#include "pinout.h" // Inclui as definições de pinos (LED_PIN)

// Função de setup para inicializar o ambiente de teste
void setUp(void) {
    // Código de inicialização, se necessário
}

// Função de teardown para limpar o ambiente de teste
void tearDown(void) {
    // Código de limpeza, se necessário
}

// Teste 1: Verifica se o LED é inicializado corretamente
void test_LED_initialization(void) {
    initModule1(); // Inicializa o módulo 1

    // Verifica se o pino do LED está configurado como OUTPUT
    // Como não há uma função para ler o modo do pino, testamos o comportamento
    digitalWrite(LED_PIN, HIGH); // Define o pino como HIGH
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN)); // Verifica se o pino está HIGH

    digitalWrite(LED_PIN, LOW); // Define o pino como LOW
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_PIN)); // Verifica se o pino está LOW
}

// Teste 2: Verifica se o LED acende e apaga corretamente
void test_LED_toggle(void) {
    initModule1(); // Inicializa o módulo 1

    // Verifica se o LED começa apagado
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_PIN));

    // Acende o LED
    digitalWrite(LED_PIN, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN));

    // Apaga o LED
    digitalWrite(LED_PIN, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_PIN));
}

// Teste 3: Verifica a função runModule1
void test_runModule1(void) {
    initModule1(); // Inicializa o módulo 1

    // Executa a função runModule1
    runModule1();

    // Verifica se o LED foi alternado (HIGH -> LOW ou LOW -> HIGH)
    int estadoInicial = digitalRead(LED_PIN);
    runModule1();
    int estadoFinal = digitalRead(LED_PIN);
    TEST_ASSERT_NOT_EQUAL(estadoInicial, estadoFinal);
}

// Função principal para rodar os testes
void runTests(void) {
    UNITY_BEGIN(); // Inicia o framework de testes

    // Executa os testes
    RUN_TEST(test_LED_initialization);
    RUN_TEST(test_LED_toggle);
    RUN_TEST(test_runModule1);

    UNITY_END(); // Finaliza o framework de testes
}

void setup() {
    // Aguarda 2 segundos para o Serial estar pronto (opcional)
    delay(2000);

    // Inicia os testes
    runTests();
}

void loop() {
    // Nada a fazer aqui
}