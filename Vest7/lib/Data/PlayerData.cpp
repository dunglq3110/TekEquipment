#include "PlayerData.h"

PlayerData::PlayerData()
{
}

void PlayerData::setMatchStatsVest(uint16_t health, uint16_t armor,
                                   uint16_t maxHealth, uint16_t maxArmor,
                                   uint8_t armorIncrease, uint8_t armorDecrease, uint8_t extraDamageRecv,
                                   uint8_t baseDamageVulnerability, uint8_t baseDamageResistance,
                                   uint8_t bonusDamageVulnerability, uint8_t bonusDamageResistance)
{
    stats.health = health;
    stats.armor = armor;
    stats.maxHealth = maxHealth;
    stats.maxArmor = maxArmor;
    stats.armorIncrease = armorIncrease;
    stats.armorDecrease = armorDecrease;
    stats.extraDamageRecv = extraDamageRecv;
    stats.baseDamageVulnerability = baseDamageVulnerability;
    stats.baseDamageResistance = baseDamageResistance;
    stats.bonusDamageVulnerability = bonusDamageVulnerability;
    stats.bonusDamageResistance = bonusDamageResistance;

    preserveStats = stats;
}

void PlayerData::resetStats()
{
    stats = {};
}