#include "Button.h"

Button::Button(int pin, callbackFunction onPressed, callbackFunction onReleased)
    : _pin(pin), _onPressed(onPressed), _onReleased(onReleased)
{
    pinMode(_pin, INPUT_PULLUP);
}

void Button::click()
{

    _reading = digitalRead(_pin);
    if (_reading != _state && (millis() - _lastDebounce >= 75))
    {
        _lastDebounce = millis();
        if (_reading == LOW)
        {
            _onPressed();
        }
        else
        {
            _onReleased();
        }
    }
    _state = _reading;
}

void Button::touch()
{
    _reading = digitalRead(_pin);
    if (_reading != _state && (millis() - _lastDebounce >= 75))
    {
        _lastDebounce = millis();
        if (_reading == HIGH)
        {
            Serial.print("Touch Button Pressed: ");
            Serial.println(_pin);
        }
        else
        {
            Serial.print("Touch Button Released: ");
            Serial.println(_pin);
        }
    }
    _state = _reading;
}