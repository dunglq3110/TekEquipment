#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <ArduinoJson.h>

class Observer
{
public:
    virtual void updateInt(uint64_t uint_64) = 0;
    virtual void updateJson(const JsonDocument &json) = 0;
    virtual void updateBinary(const uint8_t *data, size_t size, uint8_t id) = 0;
};

#endif