#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <ArduinoJson.h>

class Observer
{
public:
    virtual void updateMesg(const std::string &message) = 0;
    virtual void updateJson(const JsonDocument &json) = 0;
    virtual void updateBinary(const uint8_t *data, size_t size) = 0;
};

#endif