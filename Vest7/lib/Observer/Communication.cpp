#include "Communication.h"
#include <Arduino.h>

void Communication::attach(Observer *observer)
{
    observers.push_back(observer);
}

void Communication::detach(Observer *observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Communication::notifyInt(uint64_t uint_64)
{
    for (Observer *observer : observers)
    {
        observer->updateInt(uint_64);
    }
}

void Communication::notifyJson(const JsonDocument &json)
{
    for (Observer *observer : observers)
    {
        observer->updateJson(json);
    }
}

void Communication::notifyBinary(const uint8_t *data, size_t size, uint8_t id)
{
    for (Observer *observer : observers)
    {
        observer->updateBinary(data, size, id);
    }
}