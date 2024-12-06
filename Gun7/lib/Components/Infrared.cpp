#include "Infrared.h"

Infrared::Infrared(int pin) : _pin(pin)
{
}

void Infrared::init()
{
    _irsend = new IRsend(_pin);
    _irsend->begin();
}

void Infrared::shot(uint8_t data)
{
    _irsend->sendRC6(data, 8);
}

void Infrared::pair(unsigned long long pairCode)
{
    _irsend->sendRC6(pairCode, 56);
}
