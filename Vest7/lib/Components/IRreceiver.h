#ifndef IRRECEIVER_H
#define IRRECEIVER_H

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <Arduino.h>
#include "Communication.h"

class IRreceiver
{
private:
    int _pin;
    IRrecv *_irrecv;
    decode_results _results;
    Communication *_communication;

public:
    IRreceiver(Communication *comm, int pin);
    void init();
    void irReceives();
};

#endif