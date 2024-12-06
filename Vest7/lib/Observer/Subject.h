#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>
#include "Observer.h"

class Subject
{
public:
    virtual void attach(Observer *observer) = 0;
    virtual void detach(Observer *observer) = 0;
    virtual void notifyInt(uint64_t uint_64) = 0;
    virtual void notifyJson(const JsonDocument &json) = 0;
    virtual void notifyBinary(const uint8_t *data, size_t size, uint8_t id) = 0;
};

#endif