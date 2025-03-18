------------------------------------------------------------
Descrição: Este arquivo contém a documentação do projeto.

O que pode ser adicionar:
Descrição do projeto.
Instruções de configuração.
Exemplos de uso.
------------------------------------------------------------

# Projeto ESP32

Este é um projeto de exemplo para o ESP32 usando PlatformIO.

## Configuração

1. Clone o repositório.
2. Abra o projeto no PlatformIO.
3. Compile e faça upload para o ESP32.

## Exemplos de Uso

- `main.cpp`: Contém o loop principal do programa.
- `modules/module1.cpp`: Exemplo de um módulo que controla um LED.

## Adicionando atalho para o pio no PowerShell:

- Set-Alias pio "C:\Users\<Seu Usuário>\.platformio\penv\Scripts\platformio.exe"

## Para executar o teste:

- pio test -vv

## Para compilar e fazer upload do programa principal:

- pio run --target upload