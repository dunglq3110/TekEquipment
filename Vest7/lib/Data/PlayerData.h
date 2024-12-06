#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include <stdint.h>
#include <esp_now.h>
#include "CommonStructs.h"

class PlayerData
{
public:
    struct VestStat
    {
        CommonStructs::MessageHeader header;

        uint16_t health = 0;
        uint16_t armor = 0;
        uint16_t maxHealth = 0;
        uint16_t maxArmor = 0;

        uint8_t armorIncrease = 0;
        uint8_t armorDecrease = 0;
        uint8_t extraDamageRecv = 0;
        uint8_t baseDamageVulnerability = 0;
        uint8_t baseDamageResistance = 0;
        uint8_t bonusDamageVulnerability = 0;
        uint8_t bonusDamageResistance = 0;
    };

    struct PlayerState
    {
        bool playerIsDead = false;
        bool playerIsIgnoreTrueDamage = false;
        bool playerIsIgnoreDebuffs = false;
        bool isGameStarted = false;
    };

    struct NetworkCredentials
    {
        const uint8_t *vestMacint;
        esp_now_peer_info_t peerInfo = {};
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
    VestStat stats;
    VestStat preserveStats;
    PlayerState state;
    MatchCredentials matchCreds;
    NetworkCredentials netCreds;

    void setMatchStatsVest(uint16_t health, uint16_t armor,
                           uint16_t maxHealth, uint16_t maxArmor,
                           uint8_t armorIncrease, uint8_t armorDecrease, uint8_t extraDamageRecv,
                           uint8_t baseDamageVulnerability, uint8_t baseDamageResistance,
                           uint8_t bonusDamageVulnerability, uint8_t bonusDamageResistance);
    void resetStats();
};

#endif