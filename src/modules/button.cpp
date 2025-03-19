#include "button.h"
#include "Arduino.h"
#include "config.h"
#include "pinout.h"
#include "utils.h"
#include "mqtt_manager.h"

Button::Button() : button_state(RELEASED_BUTTON),
                   edge(FALLING_BUTTON),
                   bouncing_time(DEBOUNCING_TIME),
                   hold_time(HOLD_TIME),
                   _bouncing_flag(false),
                   _hold_flag(false)
{
    initButton();
}
Button::~Button() {}

void Button::initButton()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void Button::runbutton()
{

    bool _buttonStatus = digitalRead(BUTTON_PIN);

    if (_buttonStatus != _lastButtonStatus)
    {
        if (!_bouncing_flag)
        {
            _bouncing_count = millis();
            _bouncing_flag = true;
        }
        else if (millis() - _bouncing_count >= bouncing_time)
        {
            button_state = (_buttonStatus == PRESSED_BUTTON) ? PRESSED_BUTTON : RELEASED_BUTTON;
            _lastButtonStatus = button_state;
            _bouncing_flag = false;
            edge = (_buttonStatus == PRESSED_BUTTON) ? RISING_BUTTON : FALLING_BUTTON;
        }
        else
        {
            // aguardando o debouncing
        }
    }
    else
    {
        _bouncing_flag = false;
    }

    if (button_state == PRESSED_BUTTON)
    {
        mqttManager->publishSwitchState(true);
        if (!_hold_flag)
        {
            _hold_count = millis();
            _hold_flag = true;
        }
        else if (millis() - _hold_count >= hold_time)
        {
            button_state = HOLD_PRESSED_BUTTON;
            _hold_flag = false;
        }
    }
    else
    {
        mqttManager->publishSwitchState(false);
        _hold_flag = false;
    }
}