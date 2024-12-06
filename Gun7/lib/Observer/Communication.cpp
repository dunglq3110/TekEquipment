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

void Communication::notifyMesg(const std::string &message)
{
    for (Observer *observer : observers)
    {
        observer->updateMesg(message);
    }
}

void Communication::notifyJson(const JsonDocument &json)
{
    for (Observer *observer : observers)
    {
        observer->updateJson(json);
    }
}

void Communication::notifyBinary(const uint8_t *data, size_t size)
{
    for (Observer *observer : observers)
    {
        observer->updateBinary(data, size);
    }
}