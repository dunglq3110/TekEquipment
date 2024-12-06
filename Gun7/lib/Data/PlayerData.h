#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include <stdint.h>
#include <ArduinoJson.h>
#include <esp_now.h>

class PlayerData
{
public:
    struct GunStat
    {
        uint16_t health = 0;
        uint16_t armor = 0;
        uint8_t damage = 0;
        uint8_t heal = 0;
        uint8_t maxBullet = 0;
        uint8_t maxSSketchBullet = 0;
        uint8_t curBullet = 0;
        uint8_t curSSketchBullet = 0;
        uint16_t bulletTime = 0;
        uint16_t sSketchBulletTime = 0;
        uint8_t lifeStealValue = 0;

        bool playerHasHeal = false;
        bool playerHasSSketch = false;
        bool playerHasTrueDamage = false;
    };

    struct PlayerState
    {
        bool playerIsUsingHeal = false;
        bool playerisUsingSSketch = false;
        bool isReloading = false;
        bool playerIsDead = false;
        bool didPlayerShoot = false;
        unsigned long reloadStartTime = 0;
        unsigned long currentSSketchRegen = 0;
    };

    struct NetworkCredentials
    {
        esp_now_peer_info_t peerInfo = {};
        const uint8_t *_gunMacInt;
    };

    struct NetworkPairingCredentail
    {
        std::string _gunMacStr;
        std::string _gunMacPairingStr;
        unsigned long long _gunMacPairingLong;
        uint8_t _channel;
        uint8_t _mac_address_holder[6];
    };

    struct MatchCredentials
    {
        uint8_t id;
        uint8_t teamId;
    };

public:
    PlayerData();

    /* Struct instances */
    GunStat stats;
    GunStat preserveStats;
    PlayerState state;
    MatchCredentials matchCreds;
    NetworkCredentials netCreds;
    NetworkPairingCredentail netPairingCreds;

    void setStats(const JsonDocument &json);
    void setHealthArmor(const JsonDocument &json);
    void resetStats();
};

#endif