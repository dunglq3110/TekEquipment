#include "DamageCalcualtion.h"

DamageCalculation::DamageCalculation() {}

uint16_t DamageCalculation::calculateDamage(uint16_t baseDamage, uint16_t armor, bool isTrueDamage,
                                            uint8_t baseDamageVulnerability, uint8_t baseDamageResistance,
                                            uint8_t bonusDamageVulnerability, uint8_t bonusDamageResistance)
{
    if (isTrueDamage)
    {
        return baseDamage;
    }

    // Combine base and bonus damage modifiers
    int16_t totalDamageModifier = (baseDamageVulnerability - baseDamageResistance) * 2 +
                                  (bonusDamageVulnerability - bonusDamageResistance);

    // Apply damage modifiers
    if (totalDamageModifier != 0)
    {
        int16_t modifiedDamage = baseDamage;
        modifiedDamage += (modifiedDamage * totalDamageModifier) / 200;
        baseDamage = (modifiedDamage > UINT16_MAX) ? UINT16_MAX : (modifiedDamage < 0) ? 0
                                                                                       : modifiedDamage;
    }

    // Apply armor reduction
    uint16_t armorReduction = (baseDamage * armor) / (100 + armor);
    return baseDamage - armorReduction;
}

uint16_t DamageCalculation::calculateDOT(uint16_t baseDamage, uint16_t armor, bool isTrueDamage,
                                         uint8_t baseDamageVulnerability, uint8_t baseDamageResistance)
{
    uint16_t damage = calculateDamage(baseDamage, armor, isTrueDamage,
                                      baseDamageVulnerability, baseDamageResistance,
                                      0, 0);
    return (damage > 255) ? 255 : damage;
}