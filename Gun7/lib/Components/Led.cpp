#include "Led.h"

Led::Led(int pin) : _pin(pin), _isOn(false)
{
    pinMode(_pin, OUTPUT);
}

void Led::on()
{
    digitalWrite(_pin, HIGH);
    _isOn = true;
}

void Led::off()
{
    digitalWrite(_pin, LOW);
    _isOn = false;
}

void Led::blink()
{
    if (millis() - _lastState >= 1000)
    {
        _lastState = millis();
        if (_isOn)
            off();
        else
            on();
    }
}