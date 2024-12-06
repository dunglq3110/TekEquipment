#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class Button
{
private:
    typedef std::function<void(void)> callbackFunction;
    callbackFunction _onPressed, _onReleased;
    int _pin;
    int _reading;
    int _state;
    unsigned long _lastDebounce;

public:
    Button(int pin, callbackFunction onPressed, callbackFunction onReleased);
    void click();
    void touch();
};

#endif