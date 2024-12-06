#include "GameManager.h"

void GameManager::sendConfirmPacket(const uint8_t *mac)
{
    if (mac == nullptr)
    {
        Serial.println("MAC address is null");
        return;
    }

    _confirmPacket.header.type = CommonStructs::MessageType::ConfirmPacket;
    _confirmPacket.id = _playerData.matchCreds.id;
    _confirmPacket.teamId = _playerData.matchCreds.teamId;

    uint8_t data[sizeof(GameStructs::ConfirmPacket)];
    memcpy(data, &_confirmPacket, sizeof(GameStructs::ConfirmPacket));

    _espNow.enqueuePlayerMessage(mac, data, sizeof(GameStructs::ConfirmPacket));
}

void GameManager::sendHealthArmorReport()
{
    _healthArmorReport = {};
    _healthArmorReport.header.type = CommonStructs::MessageType::HealthArmorReport;
    _healthArmorReport.id = _playerData.matchCreds.id;
    _healthArmorReport.health = _playerData.stats.health;
    _healthArmorReport.armor = _playerData.stats.armor;

    uint8_t buffer[sizeof(GameStructs::HealthArmorReport)];
    memcpy(buffer, &_healthArmorReport, sizeof(GameStructs::HealthArmorReport));

    _espNow.enqueueGunMessage(buffer, sizeof(GameStructs::HealthArmorReport));
}

void GameManager::sendDamageReport(uint8_t id, uint16_t damage)
{
    _damageReport = {};
    _damageReport.header.type = CommonStructs::MessageType::DamageReport;
    _damageReport.victimId = _playerData.matchCreds.id;
    _damageReport.taggerId = id;
    _damageReport.damage = damage;

    // Debug print
    Serial.print("Sending damage report: ");
    Serial.print("Victim: ");
    Serial.print(_damageReport.victimId);
    Serial.print(", Tagger: ");
    Serial.print(_damageReport.taggerId);
    Serial.print(", My ID: ");
    Serial.print(_playerData.matchCreds.id);
    Serial.print(", Damage: ");
    Serial.println(_damageReport.damage);

    uint8_t buffer[sizeof(GameStructs::DamageReport)];
    memcpy(buffer, &_damageReport, sizeof(GameStructs::DamageReport));

    _espNow.enqueueGunMessage(buffer, sizeof(GameStructs::DamageReport));
}

void GameManager::sendHealingReport(uint8_t id, uint8_t healing)
{
    _healingReport = {};
    _healingReport.header.type = CommonStructs::MessageType::HealingReport;
    _healingReport.healedId = _playerData.matchCreds.id;
    _healingReport.healerId = id;
    _healingReport.heal = healing;

    uint8_t buffer[sizeof(GameStructs::HealingReport)];
    memcpy(buffer, &_healingReport, sizeof(GameStructs::HealingReport));

    _espNow.enqueueGunMessage(buffer, sizeof(GameStructs::HealingReport));
}

void GameManager::sendSSketchReport(uint8_t id, uint16_t damage)
{
    _sSketchReport = {};
    _sSketchReport.header.type = CommonStructs::MessageType::SSketchReport;
    _sSketchReport.victimId = _playerData.matchCreds.id;
    _sSketchReport.taggerId = id;
    _sSketchReport.damage = damage;

    uint8_t buffer[sizeof(GameStructs::SSketchReport)];
    memcpy(buffer, &_sSketchReport, sizeof(GameStructs::SSketchReport));

    _espNow.enqueueGunMessage(buffer, sizeof(GameStructs::SSketchReport));
}

void GameManager::sendIsDeadReport()
{
    GameStructs::DeadPacket deadPacket;
    deadPacket.header.type = CommonStructs::MessageType::IsDeadReport;

    uint8_t buffer[sizeof(GameStructs::DeadPacket)];
    memcpy(buffer, &deadPacket, sizeof(GameStructs::DeadPacket));

    _espNow.enqueueGunMessage(buffer, sizeof(GameStructs::DeadPacket));
}