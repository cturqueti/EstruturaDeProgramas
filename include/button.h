#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define DEBOUNCING_TIME 50
#define HOLD_TIME 1000

enum ButtonState
{
    PRESSED_BUTTON,
    RELEASED_BUTTON,
    HOLD_PRESSED_BUTTON
};

enum Edge
{
    RISING_BUTTON,
    FALLING_BUTTON
};

class Button
{
public:
    ButtonState button_state;
    Edge edge;
    unsigned long hold_time, bouncing_time;

    Button();  // Construtor
    ~Button(); // Destrutor

    void initButton(); // Inicializa a conexão Wi-Fi
    void runbutton();  // Mantém a conexão Wi-Fi ativa

    inline bool getButtonStatus() { return button_state; }

private:
    bool _lastButtonStatus; // Variável para armazenar o estado anterior do botão
    bool _hold_flag, _bouncing_flag;
    unsigned long _hold_count, _bouncing_count;
};

#endif // BUTTON_H