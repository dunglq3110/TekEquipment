#ifndef DAMAGECALCULATION_H
#define DAMAGECALCULATION_H

#include <stdint.h>

class DamageCalculation
{
private:
public:
    DamageCalculation();
    uint16_t calculateDamage(uint16_t baseDamage, uint16_t armor, bool isTrueDamage,
                             uint8_t baseDamageVulnerability, uint8_t baseDamageResistance,
                             uint8_t bonusDamageVulnerability, uint8_t bonusDamageResistance);

    uint16_t calculateDOT(uint16_t baseDamage, uint16_t armor, bool isTrueDamage,
                          uint8_t baseDamageVulnerability, uint8_t baseDamageResistance);
};

#endif