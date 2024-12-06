#ifndef INFRARED_H
#define INFRARED_H

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "cstdlib"
#include "string"

class Infrared
{
private:
    IRsend *_irsend;
    int _pin;

public:
    Infrared(int pin);
    void init();
    void shot(uint8_t data);
    void pair(unsigned long long pairCode);
};

#endif