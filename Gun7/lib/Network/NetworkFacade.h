#ifndef NETWORKFACADE_H
#define NETWORKFACADE_H

#include "EspNow.h"
#include "WServer.h"
#include "WSocket.h"
#include "PlayerData.h"
#include "PingPong.h"

class NetworkFacade
{
private:
    Communication *_communication;
    PlayerData *_playerData;
    EspNow *_espNow;
    WServer *_wServer;
    WSocket *_wSocket;
    PingPong *_ping;

public:
    NetworkFacade(Communication *comm, WServer *ser, EspNow *now, WSocket *soc, PlayerData *data, PingPong *ping);
    void initPreGame();
    void wServerHandlingClient();
    void espnowHandlingPeer();
    bool isVestPaired();
    bool isPhoneConnected();
    void onPingReceived();
    void sendMessageEspNowLoop();
    void sendBinaryToWSocketLoop();
    void forwardJsonToWSocket(const JsonDocument &json);
    void forwardBinaryToWSocket(const uint8_t *data, size_t len);
    void enqueueMessageToEspNowVest(const uint8_t *data, size_t len);
    void enqueueMessageToEspNowPlayer(const uint8_t *data, const uint8_t *mac, size_t len);
};

#endif