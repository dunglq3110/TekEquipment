#include "IRReceiver.h"

IRreceiver::IRreceiver(Communication *comm, int pin) : _communication(comm), _pin(pin)
{
}
void IRreceiver::init()
{
    _irrecv = new IRrecv(_pin);
    _irrecv->enableIRIn();
}

void IRreceiver::irReceives()
{
    if (_irrecv->decode(&_results))
    {
        Serial.print("IR Receiver received value locally: ");
        Serial.println(_results.value, HEX);
        _communication->notifyInt(_results.value);
        _irrecv->resume();
    }
}
