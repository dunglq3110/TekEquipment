#include "Effects.h"

Effects::Effects(Communication *comm) : _communication(comm)
{
}

void Effects::setRegen(uint8_t value, uint16_t interval)
{
    _regen.value = value;
    _regen.interval = interval * 1000;
}

void Effects::setFire(uint8_t id, uint8_t level, uint8_t duration, uint8_t value, bool isFireTrueDamage)
{
    if (level > 0 && value > 0 && (_fire.level <= level || _fire.value <= value))
    {
        resetFire();
        Serial.println("Fire set with higher lvl");

        _fire.header.header.type = CommonStructs::MessageType::Debuff;
        _fire.header.debuffType = CommonStructs::DebuffType::Fire;
        _fire.ownerId = id;
        _fire.level = level;
        _fire.duration = duration;
        _fire.value = value;
        _fire.hasTrueDamage = isFireTrueDamage;
        _state.isFire = true;
        _timer.lastFire = millis();

        sendFire();
    }
    else
        Serial.println("Fire not worth setting");
}

void Effects::setPoison(uint8_t id, uint8_t level, uint8_t duration, uint8_t value)
{
    if (level > 0 && value > 0 && (_poison.level <= level || _poison.value <= value))
    {
        resetPoison();
        Serial.println("Poison set with higher lvl");

        _poison.header.header.type = CommonStructs::MessageType::Debuff;
        _poison.header.debuffType = CommonStructs::DebuffType::Poison;
        _poison.ownerId = id;
        _poison.level = level;
        _poison.duration = duration;
        _poison.value = value;
        _state.isPoison = true;
        _timer.lastPoison = millis();

        sendPoison();
    }
    else
        Serial.println("Poison not worth setting");
}

void Effects::setDeHeal(uint8_t id, uint8_t level, uint8_t duration, uint8_t healReduction, bool blockRegen)
{
    if (level > 0 && healReduction > 0 && (_deHeal.level <= level || _deHeal.healReduction <= healReduction))
    {
        resetDeHeal();
        Serial.println("DeHeal set with higher lvl");

        _deHeal.header.header.type = CommonStructs::MessageType::Debuff;
        _deHeal.header.debuffType = CommonStructs::DebuffType::DeHeal;
        _deHeal.ownerId = id;
        _deHeal.level = level;
        _deHeal.duration = duration;
        _deHeal.healReduction = healReduction;
        _deHeal.blockRegen = blockRegen;
        _state.isDeHeal = true;
        _timer.lastDeHeal = millis();

        sendDeHeal();
    }
    else
        Serial.println("DeHeal not worth setting");
}

void Effects::setSilence(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t nvalue, uint8_t &armorDecrease)
{
    if (nlevel > 0 && nvalue > 0 && (_silence.level <= nlevel || _silence.armorDecrease <= nvalue))
    {
        resetSilence(armorDecrease);
        Serial.println("Silence set with higher lvl");

        _silence.header.header.type = CommonStructs::MessageType::Debuff;
        _silence.header.debuffType = CommonStructs::DebuffType::Silence;
        _silence.ownerId = id;
        _silence.level = nlevel;
        _silence.duration = duration;
        _silence.armorDecrease = nvalue;
        armorDecrease -= _silence.armorDecrease;
        _timer.lastSilence = millis();

        sendSilence();
    }
    else
        Serial.println("Silence not worth setting");
}
void Effects::setExposed(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t avalue, uint8_t nvalue, uint8_t &armorDecrease, uint8_t &bonusDamageVulerability)
{
    if (nlevel > 0 && nvalue > 0 && (_exposed.level <= nlevel || _exposed.damageVulnerability <= nvalue))
    {
        resetExposed(armorDecrease, bonusDamageVulerability);
        Serial.println("Exposed set with higher lvl: ");

        _exposed.header.header.type = CommonStructs::MessageType::Debuff;
        _exposed.header.debuffType = CommonStructs::DebuffType::Exposed;
        _exposed.ownerId = id;
        _exposed.level = nlevel;
        _exposed.duration = duration;
        _exposed.damageVulnerability = nvalue;
        _exposed.armorDecrease = avalue;
        armorDecrease -= _exposed.armorDecrease;
        bonusDamageVulerability += _exposed.damageVulnerability;
        _state.isExposed = true;
        _timer.lastExposed = millis();

        sendExposed();
    }
    else
        Serial.println("Exposed not worth setting");
}

void Effects::setShattered(uint8_t id, uint8_t nlevel, uint8_t duration, uint8_t avalue, uint8_t nvalue, uint8_t &bonusArmor, uint8_t &extraDamageRecv)
{
    if (nlevel > 0 && nvalue > 0 && (_shattered.level <= nlevel || _shattered.extraDamageRecv <= nvalue))
    {
        resetShattered(extraDamageRecv, bonusArmor);
        Serial.println("Shattered set with higher lvl: ");

        _shattered.header.header.type = CommonStructs::MessageType::Debuff;
        _shattered.header.debuffType = CommonStructs::DebuffType::Shattered;
        _shattered.ownerId = id;
        _shattered.level = nlevel;
        _shattered.duration = duration;
        _shattered.extraDamageRecv = nvalue;
        extraDamageRecv += _shattered.extraDamageRecv;
        bonusArmor += _shattered.bonusArmor;
        _state.isShattered = true;
        _timer.lastShattered = millis();

        sendShattered();
    }
    else
        Serial.println("Shattered not worth setting");
}

void Effects::resetFire()
{
    _timer.lastFire = 0;
    _fire.ownerId = 0;
    _fire.level = 0;
    _fire.duration = 0;
    _fire.value = 0;
    _fire.hasTrueDamage = false;
    _state.isFire = false;
}

void Effects::resetPoison()
{
    _timer.lastPoison = 0;
    _poison.ownerId = 0;
    _poison.level = 0;
    _poison.duration = 0;
    _poison.value = 0;
    _state.isPoison = false;
}

void Effects::resetDeHeal()
{
    _timer.lastDeHeal = 0;
    _deHeal.ownerId = 0;
    _deHeal.level = 0;
    _deHeal.duration = 0;
    _state.isDeHeal = false;
    _state.isDeRegen = false;
}

void Effects::resetSilence(uint8_t &armorDecrease)
{
    _timer.lastSilence = 0;
    _silence.ownerId = 0;
    _silence.level = 0;
    _silence.duration = 0;
    _silence.armorDecrease = 0;
    _state.isSilence = false;
    armorDecrease += _silence.armorDecrease;
}

void Effects::resetExposed(uint8_t &armorDecrease, uint8_t &bonusDamageVulerability)
{
    _timer.lastExposed = 0;
    _exposed.ownerId = 0;
    _exposed.level = 0;
    _exposed.duration = 0;
    _exposed.armorDecrease = 0;
    _exposed.damageVulnerability = 0;
    _state.isExposed = false;

    armorDecrease += _exposed.armorDecrease;
    bonusDamageVulerability -= _exposed.damageVulnerability;
}

void Effects::resetShattered(uint8_t &extraDamageRecv, uint8_t &bonusArmor)
{
    _timer.lastShattered = 0;
    _shattered.ownerId = 0;
    _shattered.level = 0;
    _shattered.duration = 0;
    _shattered.bonusArmor = 0;
    _shattered.extraDamageRecv = 0;

    extraDamageRecv -= _shattered.extraDamageRecv;
    bonusArmor -= _shattered.bonusArmor;
}

void Effects::regenCountdown(uint16_t &health)
{
    if (millis() - _regen.lastRegen >= _regen.interval && _regen.value > 0)
    {
        _regen.lastRegen = millis();
        if (_deHeal.blockRegen)
            return;
        health += _regen.value;
    }
}

void Effects::fireDebuffCountdown(uint16_t &health)
{
    if (millis() - _timer.lastFire >= _interval.fire && _fire.duration > 0)
    {
        _fire.duration--;
        health -= _fire.value;
        sendFire();
        _timer.lastFire = millis();
        if (_fire.duration <= 0)
        {
            resetFire();
        }
    }
}

void Effects::poisonDebuffCountdown(uint16_t &health)
{
    if (millis() - _timer.lastPoison >= _interval.poison && _poison.duration > 0)
    {
        _poison.duration--;
        health -= _poison.value;
        sendPoison();
        _timer.lastPoison = millis();
        if (_poison.duration <= 0)
        {
            resetPoison();
        }
    }
}

void Effects::deHealDebuffCountdown(uint8_t &healReduction)
{
    if (millis() - _timer.lastDeHeal >= _interval.deHeal && _deHeal.duration > 0)
    {
        _deHeal.duration--;
        sendDeHeal();
        _timer.lastDeHeal = millis();
        if (_deHeal.duration <= 0)
        {
            resetDeHeal();
        }
    }
}

void Effects::silenceDebuffCountdown(uint8_t &armorDecrease)
{
    if (millis() - _timer.lastSilence >= _interval.silence && _silence.duration > 0)
    {
        _silence.duration--;
        sendSilence();
        _timer.lastSilence = millis();
        if (_silence.duration <= 0)
        {
            resetSilence(armorDecrease);
        }
    }
}

void Effects::exposedDebuffCountdown(uint8_t &armorDecrease, uint8_t &bonusDamageVulerability)
{
    if (millis() - _timer.lastExposed >= _interval.exposed && _exposed.duration > 0)
    {
        _exposed.duration--;
        sendExposed();
        _timer.lastExposed = millis();
        if (_exposed.duration <= 0)
        {
            resetExposed(armorDecrease, bonusDamageVulerability);
        }
    }
}

void Effects::shatteredDebuffCountdown(uint8_t &extraDamageRecv, uint8_t &bonusArmor)
{
    if (millis() - _timer.lastShattered >= _interval.shattered && _shattered.duration > 0)
    {
        _shattered.duration--;
        sendShattered();
        _timer.lastShattered = millis();
        if (_shattered.duration <= 0)
        {
            resetShattered(extraDamageRecv, bonusArmor);
        }
    }
}

void Effects::runEffects(uint16_t &health, uint8_t &armorIncrease, uint8_t healReduction, uint8_t &armorDecrease, uint8_t &extraDamageRecv, uint8_t &bonusDamageVulerability)
{
    fireDebuffCountdown(health);
    poisonDebuffCountdown(health);
    deHealDebuffCountdown(healReduction);
    silenceDebuffCountdown(armorDecrease);
    exposedDebuffCountdown(armorDecrease, bonusDamageVulerability);
    shatteredDebuffCountdown(extraDamageRecv, armorIncrease);
    regenCountdown(health);
}

void Effects::resetEffects()
{
    _fire = {};
    _poison = {};
    _deHeal = {};
    _silence = {};
    _exposed = {};
    _shattered = {};
    _timer = {};
    _interval = {};
    _state = {};
}

void Effects::serializeFire(const CommonStructs::FireDebuff &fire, uint8_t *buffer)
{
    memcpy(buffer, &fire, sizeof(CommonStructs::FireDebuff));
}

void Effects::serializePoison(const CommonStructs::PoisonDebuff &poison, uint8_t *buffer)
{
    memcpy(buffer, &poison, sizeof(CommonStructs::PoisonDebuff));
}

void Effects::serializeDeHeal(const CommonStructs::DeHealDebuff &deHeal, uint8_t *buffer)
{
    memcpy(buffer, &deHeal, sizeof(CommonStructs::DeHealDebuff));
}

void Effects::serializeSilence(const CommonStructs::SilenceDebuff &silence, uint8_t *buffer)
{
    memcpy(buffer, &silence, sizeof(CommonStructs::SilenceDebuff));
}

void Effects::serializeExposed(const CommonStructs::ExposedDebuff &exposed, uint8_t *buffer)
{
    memcpy(buffer, &exposed, sizeof(CommonStructs::ExposedDebuff));
}

void Effects::serializeShattered(const CommonStructs::ShartteredDebuff &shattered, uint8_t *buffer)
{
    memcpy(buffer, &shattered, sizeof(CommonStructs::ShartteredDebuff));
}

void Effects::sendFire()
{
    uint8_t buffer[sizeof(CommonStructs::FireDebuff)];
    serializeFire(_fire, buffer);
    _communication->notifyBinary(buffer, sizeof(buffer), _fire.ownerId);
}

void Effects::sendPoison()
{
    uint8_t buffer[sizeof(CommonStructs::PoisonDebuff)];
    serializePoison(_poison, buffer);
    _communication->notifyBinary(buffer, sizeof(CommonStructs::PoisonDebuff), _poison.ownerId);
}

void Effects::sendDeHeal()
{
    uint8_t buffer[sizeof(CommonStructs::DeHealDebuff)];
    serializeDeHeal(_deHeal, buffer);
    _communication->notifyBinary(buffer, sizeof(CommonStructs::DeHealDebuff), _deHeal.ownerId);
}

void Effects::sendSilence()
{
    uint8_t buffer[sizeof(CommonStructs::SilenceDebuff)];
    serializeSilence(_silence, buffer);
    _communication->notifyBinary(buffer, sizeof(CommonStructs::SilenceDebuff), _silence.ownerId);
}

void Effects::sendExposed()
{
    uint8_t buffer[sizeof(CommonStructs::ExposedDebuff)];
    serializeExposed(_exposed, buffer);
    _communication->notifyBinary(buffer, sizeof(CommonStructs::ExposedDebuff), _exposed.ownerId);
}

void Effects::sendShattered()
{
    uint8_t buffer[sizeof(CommonStructs::ShartteredDebuff)];
    serializeShattered(_shattered, buffer);
    _communication->notifyBinary(buffer, sizeof(CommonStructs::ShartteredDebuff), _shattered.ownerId);
}

uint8_t Effects::returnHealReductionValue()
{
    return _deHeal.healReduction;
}