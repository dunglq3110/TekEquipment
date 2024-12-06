#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdint.h>
#include "CommonStructs.h"
#include "Communication.h"

class Effects
{
private:
    struct DebuffTimers
    {
        unsigned long long lastFire;
        unsigned long long lastPoison;
        unsigned long long lastDeHeal;
        unsigned long long lastSilence;
        unsigned long long lastExposed;
        unsigned long long lastShattered;
    };

    struct DebuffTickingInterval
    {
        uint16_t fire = 1000;
        uint16_t poison = 1000;
        uint16_t deHeal = 1000;
        uint16_t silence = 1000;
        uint16_t exposed = 1000;
        uint16_t shattered = 1000;
    };

    struct DebuffState
    {
        bool isShattered = false;
        bool isExposed = false;
        bool isSilence = false;
        bool isDeHeal = false;
        bool isDeRegen = false;
        bool isPoison = false;
        bool isFire = false;
    };

    Communication *_communication;
    DebuffTimers _timer;
    DebuffTickingInterval _interval;
    DebuffState _state;

    CommonStructs::Regeneration _regen;
    CommonStructs::FireDebuff _fire;
    CommonStructs::PoisonDebuff _poison;
    CommonStructs::DeHealDebuff _deHeal;
    CommonStructs::SilenceDebuff _silence;
    CommonStructs::ExposedDebuff _exposed;
    CommonStructs::ShartteredDebuff _shattered;

public:
    Effects(Communication *comm);

    void setRegen(uint8_t nvalue, uint16_t interval);
    void setFire(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t nvalue, bool isFireTrueDamage);
    void setPoison(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t nvalue);
    void setDeHeal(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t nvalue, bool isBlockRegen);
    void setSilence(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t nvalue, uint8_t &armorDecrease);
    void setExposed(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t avalue, uint8_t nvalue, uint8_t &armorDecrease, uint8_t &bonusDamageVulerability);
    void setShattered(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t avalue, uint8_t nvalue, uint8_t &bonusArmor, uint8_t &extraDamageRecv);

    void resetFire();
    void resetPoison();
    void resetDeHeal();
    void resetSilence(uint8_t &armorDecrease);
    void resetExposed(uint8_t &armorDecrease, uint8_t &bonusDamageVulerability);
    void resetShattered(uint8_t &extraDamageRecv, uint8_t &bonusArmor);

    void regenCountdown(uint16_t &health);
    void fireDebuffCountdown(uint16_t &health);
    void poisonDebuffCountdown(uint16_t &health);
    void deHealDebuffCountdown(uint8_t &healReduction);
    void silenceDebuffCountdown(uint8_t &armorDecrease);
    void exposedDebuffCountdown(uint8_t &armorDecrease, uint8_t &bonusDamageVulerability);
    void shatteredDebuffCountdown(uint8_t &extraDamageRecv, uint8_t &bonusArmor);

    void serializeFire(const CommonStructs::FireDebuff &fire, uint8_t *buffer);
    void serializePoison(const CommonStructs::PoisonDebuff &poison, uint8_t *buffer);
    void serializeDeHeal(const CommonStructs::DeHealDebuff &deHeal, uint8_t *buffer);
    void serializeSilence(const CommonStructs::SilenceDebuff &silence, uint8_t *buffer);
    void serializeExposed(const CommonStructs::ExposedDebuff &exposed, uint8_t *buffer);
    void serializeShattered(const CommonStructs::ShartteredDebuff &shartted, uint8_t *buffer);

    void sendFire();
    void sendPoison();
    void sendDeHeal();
    void sendSilence();
    void sendExposed();
    void sendShattered();
    uint8_t returnHealReductionValue();

    void runEffects(uint16_t &health, uint8_t &armorIncrease, uint8_t healReduction, uint8_t &armorDecrease, uint8_t &extraDamageRecv, uint8_t &bonusDamageVulerability);
    void resetEffects();
};

#endif