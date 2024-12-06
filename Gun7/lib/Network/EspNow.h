#ifndef ESPNOW_H
#define ESPNOW_H

#include "Communication.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include "GameStructs.h"
#include "PlayerData.h"
#include "PingPong.h"
#include <queue>

class EspNow // Gun
{
private:
    Communication *_communication;
    PlayerData *_playerData;
    PingPong *_ping;
    JsonDocument _jsonSent;
    static EspNow *_instance;

    unsigned long long _lastEspNowSendTime;

    // Message queue, avoiding maximum bandwidth when sending at once
    std::queue<std::pair<std::vector<uint8_t>, size_t>> _vestMessageQueue;
    std::queue<std::pair<std::vector<uint8_t>, std::pair<std::vector<uint8_t>, size_t>>> _playerMessageQueue;

    const char *_enumIndexes[16] = {"Ping", "Pong", "Debuff", "HealthArmorReport", "ConfirmPacket",
                                    "DamagePacket", "HealingPacket", "SSketchPacket", "DamageReport", "HealingReport", "SSketchReport",
                                    "BulletReport", "Pairing", "MappingPlayers", "VestStatPacket", "IsDeadReport"};
    const char *_debuffIndexes[6] = {"Fire", "Poison", "DeHeal", "Silence", "Exposed", "Shattered"};

private:
    /* Callback functions */
    void onDataRecv(const uint8_t *mac, const uint8_t *data, int len);
    void onDataSent(const uint8_t *mac, esp_now_send_status_t status);
    std::vector<uint8_t> _remainingData;

    /* Helper Fuctions */
    const uint8_t *getGunMacInt();
    std::string getGunMacStr() const;
    std::string getGunMacPairingStr() const;
    unsigned long long macIntoDecimalLong(const std::string &mac) const;
    unsigned long long getGunMacParingLong() const;

public:
    /* Static related functions, need for the Esp-Now library to work */
    explicit EspNow(Communication *comm, PlayerData *playerData, PingPong *ping);
    static void onDataRecvStatic(const uint8_t *mac, const uint8_t *data, int len);
    static void onDataSentStatic(const uint8_t *mac, esp_now_send_status_t status);

    void processBatchedData(const uint8_t *data, size_t len);

    /* Central functions */
    void init();
    void peerRegister(const uint8_t *mac);
    void sendToEspNowVest();
    void sendToEspNowPlayer();
    void sendMessageLoop();
    void enqueuePlayerMessage(const uint8_t *mac, const uint8_t *data, size_t len);
    void enqueueVestMessage(const uint8_t *data, size_t len);

    /* Flow control functions */
    bool isVestPaired() const;
    void setChannel(uint8_t channel);

    /* Helper Fuctions */
    void setCredentials();
};

#endif