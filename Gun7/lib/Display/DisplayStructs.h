#ifndef DISPLAYSTRUCTS_H
#define DISPLAYSTRUCTS_H

#include <stdint.h>
#include <string>

class DisplayStructs
{
public:
    struct DebuffHousing
    {
        uint8_t fireLvl = 0;
        uint8_t poisonLvl = 0;
        uint8_t deHealLvl = 0;
        uint8_t silenceLvl = 0;
        uint8_t exposedLvl = 0;
        uint8_t shatteredLvl = 0;
        uint8_t bleedingLvl = 0;
        uint8_t weakenedLvl = 0;
        uint8_t engulfedLvl = 0;

        uint8_t fireDuration = 0;
        uint8_t poisonDuration = 0;
        uint8_t deHealDuration = 0;
        uint8_t silenceDuration = 0;
        uint8_t exposedDuration = 0;
        uint8_t shatteredDuration = 0;
        uint8_t bleedingDuration = 0;
        uint8_t weakenedDuration = 0;
        uint8_t engulfedDuration = 0;
    };

    struct StatsHousing
    {
        uint16_t health = 0;
        uint16_t armor = 0;
        uint16_t damage = 0;
        uint8_t regenAmount = 0;
        uint8_t normalBullet = 0;
        uint8_t maxNormalBullet = 0;
        uint8_t sSketchBullet = 0;
        uint8_t maxSSketchBullet = 0;
        bool hasHealing;
        bool isHealing;
        bool hasSSketch;
        bool isSSketch;
    };

    struct StateHousing
    {
        bool isDead;
        bool isVest;
        bool isPhone;
        bool isReady;
        std::string ip;
        std::string channel;
        std::string macAddress;
        bool isWiFiConnected;
        bool isWiFiPlayable;

        bool operator==(const StateHousing &state) const
        {
            return isDead == state.isDead &&
                   isVest == state.isVest &&
                   isPhone == state.isPhone &&
                   isReady == state.isReady &&
                   ip == state.ip &&
                   channel == state.channel &&
                   macAddress == state.macAddress &&
                   isWiFiConnected == state.isWiFiConnected &&
                   isWiFiPlayable == state.isWiFiPlayable;
        }
    };
};

#endif