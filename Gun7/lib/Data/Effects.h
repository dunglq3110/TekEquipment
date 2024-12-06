#ifndef EFFECTS_H
#define EFFECTS_H
#include "CommonStructs.h"
#include <ArduinoJson.h>

class Effects
{
public:
    struct DebuffRates
    {
        uint8_t fire = 0;
        uint8_t poison = 0;
        uint8_t silence = 0;
    };

    // Only contains Level and Duration of a debuff,
    // as it is the only thing that needs to be displayed
    struct DebuffDisplaySorage
    {
        uint8_t fireLvl = 0;
        uint8_t poisonLvl = 0;
        uint8_t deHealLvl = 0;
        uint8_t silenceLvl = 0;
        uint8_t exposedLvl = 0;
        uint8_t shatteredLvl = 0;

        uint8_t fireDur = 0;
        uint8_t poisonDur = 0;
        uint8_t deHealDur = 0;
        uint8_t silenceDur = 0;
        uint8_t exposedDur = 0;
        uint8_t shatteredDur = 0;
    };

    DebuffRates rates;
    DebuffDisplaySorage debuffDisplayStorage;

    CommonStructs::FireDebuff fireOrg;            // For store original value
    CommonStructs::PoisonDebuff poisonOrg;        // For store original value
    CommonStructs::DeHealDebuff deHealOrg;        // For store original value
    CommonStructs::SilenceDebuff silenceOrg;      // For store original value
    CommonStructs::ExposedDebuff exposedOrg;      // For store original value
    CommonStructs::ShartteredDebuff shatteredOrg; // For store original value

    CommonStructs::FireDebuff fireMod;       // For store modified value
    CommonStructs::PoisonDebuff poisonMod;   // For store modified value
    CommonStructs::DeHealDebuff deHealMod;   // For store modified value
    CommonStructs::SilenceDebuff silenceMod; // For store modified value

public:
    Effects();
    void setDebuffs(const JsonDocument &json);
    void calculateDebuff();
    void resetEffects();
};
#endif