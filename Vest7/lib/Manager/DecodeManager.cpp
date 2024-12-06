#include "GameManager.h"

void GameManager::decodePlayerMap(const uint8_t *buffer)
{
    GameStructs::MappingPlayers mappingPlayers;
    if (deserialize(mappingPlayers, buffer))
        playerRegistering(mappingPlayers.id, mappingPlayers.teamId, mappingPlayers.gunMacAddr, mappingPlayers.vestMacAddr);
}

void GameManager::decodeVestStatPacket(const uint8_t *buffer)
{
    GameStructs::VestStatPacket vestStatPacket;
    if (deserialize(vestStatPacket, buffer))
        _playerData.setMatchStatsVest(vestStatPacket.health, vestStatPacket.armor, vestStatPacket.maxHealth, vestStatPacket.maxArmor,
                                      vestStatPacket.armorIncrease, vestStatPacket.armorDecrease, vestStatPacket.extraDamageRecv,
                                      vestStatPacket.baseDamageVulnerability, vestStatPacket.baseDamageResistance,
                                      vestStatPacket.bonusDamageVulnerability, vestStatPacket.bonusDamageResistance);
}

void GameManager::decodeDamagePacket(const uint8_t *buffer)
{
    _damagePacket = {};
    _fire = {};
    _poison = {};
    _deHeal = {};
    _silence = {};
    if (deserialize(_damagePacket, buffer) && _damagePacket.header.type == CommonStructs::MessageType::DamagePacket)
    {
        uint8_t id = _damagePacket.id;
        uint8_t teamId = _damagePacket.teamId;
        uint16_t damage = _damagePacket.damage;
        uint8_t lifeStealValue = _damagePacket.lifeStealValue;

        // Debug print
        Serial.print("Damage packet received: ");
        Serial.print("ID: ");
        Serial.print(id);
        Serial.print(", Team ID: ");
        Serial.print(teamId);
        Serial.print(", Damage: ");
        Serial.print(damage);
        Serial.print(", My ID: ");
        Serial.print(_playerData.matchCreds.id);
        Serial.print(", My Team ID: ");
        Serial.println(_playerData.matchCreds.teamId);

        _fire = _damagePacket.fire;
        _poison = _damagePacket.poison;
        _deHeal = _damagePacket.deHeal;
        _silence = _damagePacket.silence;

        uint16_t damageDealt = _damageCalculation.calculateDamage(damage, _playerData.stats.armor, false,
                                                                  _playerData.stats.baseDamageVulnerability, _playerData.stats.baseDamageResistance,
                                                                  _playerData.stats.bonusDamageVulnerability, _playerData.stats.bonusDamageResistance);
        _playerData.stats.health -= damageDealt;

        _effect.setFire(id, _fire.level, _fire.duration, _fire.value, _fire.hasTrueDamage);
        _effect.setPoison(id, _poison.level, _poison.duration, _poison.value);
        _effect.setDeHeal(id, _deHeal.level, _deHeal.duration, _deHeal.healReduction, _deHeal.blockRegen);
        _effect.setSilence(id, _silence.level, _silence.duration, _silence.armorDecrease, _playerData.stats.armorDecrease);

        sendDamageReport(id, damageDealt);
        sendHealthArmorReport();
    }
}

void GameManager::decodeHealingPacket(const uint8_t *buffer)
{
    _healingPacket = {};
    if (deserialize(_healingPacket, buffer) && _healingPacket.header.type == CommonStructs::MessageType::HealingPacket)
    {
        uint8_t id = _healingPacket.id;
        uint8_t teamId = _healingPacket.teamId;
        uint8_t heal = _healingPacket.heal;

        _playerData.stats.health += heal;
        if (_playerData.stats.health > _playerData.stats.maxHealth)
            _playerData.stats.health = _playerData.stats.maxHealth;

        sendHealingReport(id, heal);
        sendHealthArmorReport();
    }
}

void GameManager::decodeSSketchPacket(const uint8_t *buffer)
{
    _sSketchPacket = {};
    _exposed = {};
    if (deserialize(_sSketchPacket, buffer) && _sSketchPacket.header.type == CommonStructs::MessageType::SSketchPacket)
    {
        uint8_t id = _sSketchPacket.id;
        uint8_t teamId = _sSketchPacket.teamId;
        uint16_t damage = _sSketchPacket.damage;
        _exposed = _sSketchPacket.exposed;

        _playerData.stats.health -= damage;

        sendSSketchReport(id, damage);
        sendHealthArmorReport();
    }
}