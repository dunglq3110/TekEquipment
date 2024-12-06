#ifndef PINGPONG_H
#define PINGPONG_H

#include <Arduino.h>
#include <esp_now.h>
#include "GameStructs.h"
#include "PlayerData.h"

class PingPong
{
private:
    unsigned long _lastPongTime = 0;
    unsigned long _pongTimeoutInterval = 3100;
    bool _isActive = false;

    PlayerData *_playerData;

public:
    PingPong(PlayerData *data);
    void start();
    void stop();
    void onResponseReceived();
    void onTimeOut();
    bool isTimeout() const;
    void sendPong();
};

#endif