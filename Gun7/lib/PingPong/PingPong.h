#ifndef PINGPONG_H
#define PINGPONG_H

#include <Arduino.h>
#include <esp_now.h>
#include "GameStructs.h"
#include "PlayerData.h"
#include "Communication.h"

class PingPong
{
private:
    unsigned long _lastPingTime = 0;
    unsigned long _pingInterval = 1000;
    unsigned long _pingTimeoutInterval = 3100;
    bool _receivedResponse = false;
    bool _isActive = false;

    PlayerData *_playerData;
    Communication *_communication;

public:
    PingPong(PlayerData *data, Communication *comm);
    void start();
    void stop();
    void update();
    void onResponseReceived();
    void onTimeOut();
    bool isTimeout() const;
    void sendPing();
};

#endif