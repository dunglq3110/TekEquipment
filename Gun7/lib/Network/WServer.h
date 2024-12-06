#ifndef WSERVER_H
#define WSERVER_H

#include "Communication.h"
#include <Preferences.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <Arduino.h>
#include <WiFi.h>
#include "EspNow.h"
#include "PlayerData.h"
#include "PingPong.h"

class WServer
{
private:
    String _ssid;
    std::string _wifiName;
    std::string _wifiPass;
    Preferences _pref;
    WebServer _server;
    uint8_t _mac[6];
    uint8_t _channel;
    EspNow *_espNowInstance;
    PlayerData *_playerData;
    Communication *_communication;
    PingPong *_ping;

public:
    WServer(Communication *comm, PlayerData *playerData, PingPong *ping);
    void init();
    void setup();
    void connect();
    void isConnected();
    void unConnected();
    void handleClient();

    uint8_t findWiFiChannel();
    uint8_t getWiFiChannel();
};

#endif