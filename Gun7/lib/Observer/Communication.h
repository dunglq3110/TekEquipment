#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "Subject.h"
#include <vector>
#include <algorithm>

class Communication : public Subject
{
private:
    std::vector<Observer *> observers;

public:
    void attach(Observer *observer) override;
    void detach(Observer *observer) override;
    void notifyMesg(const std::string &message) override;
    void notifyJson(const JsonDocument &json) override;
    void notifyBinary(const uint8_t *data, size_t size) override;
};

#endif