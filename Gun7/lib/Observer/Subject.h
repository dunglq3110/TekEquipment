#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>
#include "Observer.h"

class Subject
{
public:
    virtual void attach(Observer *observer) = 0;
    virtual void detach(Observer *observer) = 0;
    virtual void notifyMesg(const std::string &message) = 0;
    virtual void notifyJson(const JsonDocument &json) = 0;
    virtual void notifyBinary(const uint8_t *data, size_t size) = 0;
};

#endif