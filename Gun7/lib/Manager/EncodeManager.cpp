#include "GameManager.h"

const uint8_t *GameManager::findMacAddressByIdInMap(uint8_t id)
{
    auto it = _playerMap.find(id);
    if (it != _playerMap.end())
    {
        const auto &playerInfo = it->second;
        Serial.println("Player for IR response found");
        return std::get<2>(playerInfo);
    }
    else
    {
        Serial.println("Player for IR response not found");
        return nullptr;
    }
}

void GameManager::sendBulletReport()
{
    _bulletReport = {};
    _bulletReport.header.type = CommonStructs::MessageType::BulletReport;
    _bulletReport.id = _playerData.matchCreds.id;
    _bulletReport.normalBullets = _playerData.stats.curBullet;
    _bulletReport.ssketchBullets = _playerData.stats.curSSketchBullet;

    uint8_t buffer[sizeof(GameStructs::BulletReport)];
    memcpy(buffer, &_bulletReport, sizeof(GameStructs::BulletReport));

    // Put in server's message queue
    _networkFacade.forwardBinaryToWSocket(buffer, sizeof(GameStructs::BulletReport));
}

void GameManager::sendUpgradeChipReport()
{
    _upgradeChipReport = {};
    uint8_t chipId = _upgradeChip.readMostCommon();
    _upgradeChipReport.header.type = CommonStructs::MessageType::UpgradeChipReport;
    _upgradeChipReport.id = _playerData.matchCreds.id;
    _upgradeChipReport.chipId = chipId;

    uint8_t buffer[sizeof(GameStructs::UpgradeChipReport)];
    memcpy(buffer, &_upgradeChipReport, sizeof(GameStructs::UpgradeChipReport));

    // Put in server's message queue
    Serial.println("Sending upgrade chip report");
    _networkFacade.forwardBinaryToWSocket(buffer, sizeof(GameStructs::UpgradeChipReport));
}

void GameManager::sendDamagePacket(const uint8_t *mac)
{
    _damagePacket = {};
    _damagePacket.header.type = CommonStructs::MessageType::DamagePacket;
    _damagePacket.id = _playerData.matchCreds.id;
    _damagePacket.teamId = _playerData.matchCreds.teamId;
    _damagePacket.damage = _playerData.stats.damage;
    _damagePacket.lifeStealValue = _playerData.stats.lifeStealValue;
    _effects.calculateDebuff();
    _damagePacket.fire = _effects.fireMod;
    _damagePacket.poison = _effects.poisonMod;
    _damagePacket.deHeal = _effects.deHealMod;
    _damagePacket.silence = _effects.silenceMod;

    uint8_t buffer[sizeof(GameStructs::DamagePacket)];
    memcpy(buffer, &_damagePacket, sizeof(GameStructs::DamagePacket));

    // Put in player's message queue
    _networkFacade.enqueueMessageToEspNowPlayer(buffer, mac, sizeof(GameStructs::DamagePacket));
}

void GameManager::sendHealingPacket(const uint8_t *mac)
{
    _healingPacket = {};
    _healingPacket.header.type = CommonStructs::MessageType::HealingPacket;
    _healingPacket.id = _playerData.matchCreds.id;
    _healingPacket.teamId = _playerData.matchCreds.teamId;
    _healingPacket.heal = _playerData.stats.heal;

    uint8_t buffer[sizeof(GameStructs::HealingPacket)];
    memcpy(buffer, &_healingPacket, sizeof(GameStructs::HealingPacket));

    // Put in player's message queue
    _networkFacade.enqueueMessageToEspNowPlayer(buffer, mac, sizeof(GameStructs::HealingPacket));
}

void GameManager::sendSSketchPacket(const uint8_t *mac)
{
    _sSketchPacket = {};
    _sSketchPacket.header.type = CommonStructs::MessageType::SSketchPacket;
    _sSketchPacket.id = _playerData.matchCreds.id;
    _sSketchPacket.teamId = _playerData.matchCreds.teamId;
    _sSketchPacket.damage = 100;
    _sSketchPacket.exposed = _effects.exposedOrg;

    uint8_t buffer[sizeof(GameStructs::SSketchPacket)];
    memcpy(buffer, &_sSketchPacket, sizeof(GameStructs::SSketchPacket));

    // Put in player's message queue
    _networkFacade.enqueueMessageToEspNowPlayer(buffer, mac, sizeof(GameStructs::SSketchPacket));
}

void GameManager::sendVestStatPacket(const JsonDocument &json)
{
    GameStructs::VestStatPacket vestStatPacket;
    vestStatPacket.header.type = CommonStructs::MessageType::VestStatPacket;
    vestStatPacket.health = json["health_max"];
    vestStatPacket.armor = json["armor_max"];
    vestStatPacket.maxHealth = json["health_max"];
    vestStatPacket.maxArmor = json["armor_max"];
    vestStatPacket.armorIncrease = json["armor_plus"];
    vestStatPacket.armorDecrease = json["armor_minus"];
    vestStatPacket.extraDamageRecv = json["extra_damage_receive"];
    vestStatPacket.baseDamageVulnerability = json["base_damage_vul"];
    vestStatPacket.baseDamageResistance = json["base_damage_res"];
    vestStatPacket.bonusDamageVulnerability = json["bonus_damage_vul"];
    vestStatPacket.bonusDamageResistance = json["bonus_damage_res"];

    uint8_t data[sizeof(GameStructs::VestStatPacket)];
    memcpy(data, &vestStatPacket, sizeof(GameStructs::VestStatPacket));

    // Put in vest's message queue
    _networkFacade.enqueueMessageToEspNowVest(data, sizeof(GameStructs::VestStatPacket));
}

void GameManager::sendMappingPlayerToVest(uint8_t id, uint8_t teamId, uint8_t *gunMacAddr, uint8_t *vestMacAddr)
{
    GameStructs::MappingPlayers mappingPlayers;

    mappingPlayers.header.type = CommonStructs::MessageType::MappingPlayers;
    mappingPlayers.id = id;
    mappingPlayers.teamId = teamId;
    memcpy(mappingPlayers.gunMacAddr, gunMacAddr, 6);
    memcpy(mappingPlayers.vestMacAddr, vestMacAddr, 6);

    uint8_t data[sizeof(GameStructs::MappingPlayers)];
    memcpy(data, &mappingPlayers, sizeof(GameStructs::MappingPlayers));

    // Put in vest message queue
    _networkFacade.enqueueMessageToEspNowVest(data, sizeof(GameStructs::MappingPlayers));
}

void GameManager::sendMacSubmission()
{
    JsonDocument json;
    json["key"] = "submit_mac";

    auto formatMAC = [](const uint8_t *macAddr) -> std::string
    {
        if (macAddr && (macAddr[0] | macAddr[1] | macAddr[2] | macAddr[3] | macAddr[4] | macAddr[5]))
        {
            char macStr[18];
            snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
                     macAddr[0], macAddr[1], macAddr[2],
                     macAddr[3], macAddr[4], macAddr[5]);
            return std::string(macStr);
        }
        return "00:00:00:00:00:00";
    };

    json["gun_mac"] = formatMAC(_playerData.netCreds._gunMacInt);

    const uint8_t *vestMac = _playerData.netCreds.peerInfo.peer_addr;
    std::string formattedVestMac = formatMAC(vestMac);
    json["vest_mac"] = formattedVestMac;
    json["is_vest_connected"] = (formattedVestMac != "00:00:00:00:00:00");

    _networkFacade.forwardJsonToWSocket(json);
}

bool GameManager::reportVestConnected()
{
    if (!_networkFacade.isVestPaired())
    {
        JsonDocument json;
        json["key"] = "failed_to_start";
        json["reason"] = "vest_not_paired";
        _networkFacade.forwardJsonToWSocket(json);
        return false;
    }
    return true;
}