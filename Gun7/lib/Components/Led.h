#ifndef LED_H
#define LED_H

#include "Arduino.h"

class Led
{
private:
    int _pin;
    bool _isOn;
    unsigned long _lastState;
public:
    Led(int pin);
    void on();
    void off();
    void blink();
};


#endif