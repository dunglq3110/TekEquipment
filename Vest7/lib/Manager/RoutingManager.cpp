#include "GameManager.h"

GameManager::GameManager() : _damageCalculation(),
                             _playerData(),
                             _communication(),
                             _pong(&_playerData),
                             _espNow(&_communication, &_playerData, &_pong),
                             _irrecv(&_communication, 14),
                             _effect(&_communication)
{
    _communication.attach(this);
}

void GameManager::init()
{
    _irrecv.init();
    _espNow.init();
}

void GameManager::updateInt(uint64_t uint_64)
{
    if (!_espNow.isGunPaired())
    {
        uint8_t mac[6];
        uint8_t channel;
        if (convertMessageToHexAndSplit(std::to_string(uint_64), mac, channel))
            _espNow.peerRegister(mac, channel);
    }
    else
    {
        if (uint_64 != _playerData.matchCreds.id)
        {
            sendConfirmPacket(findMacAddressByIdInMap(uint_64));
        }
    }
}

void GameManager::updateJson(const JsonDocument &json) {}

void GameManager::updateBinary(const uint8_t *data, size_t size, uint8_t id)
{
    _header.type = static_cast<CommonStructs::MessageType>(data[0] & 0x0F);
    if (_header.type != CommonStructs::MessageType::Ping)
    {
        Serial.print("Received header via binary message notify: ");
        Serial.print(_enumIndexes[static_cast<int>(_header.type)]);
        Serial.print("\tLength: ");
        Serial.println(size);
    }

    switch (_header.type)
    {
    case CommonStructs::MessageType::Ping:
        _pong.onResponseReceived();
        break;
    case CommonStructs::MessageType::MappingPlayers:
        decodePlayerMap(data);
        break;
    case CommonStructs::MessageType::VestStatPacket:
        decodeVestStatPacket(data);
        registerPlayersAsPeers();
        _playerData.state.isGameStarted = true;
        break;
    case CommonStructs::MessageType::DamagePacket:
        decodeDamagePacket(data);
        break;
    case CommonStructs::MessageType::HealingPacket:
        decodeHealingPacket(data);
        break;
    case CommonStructs::MessageType::SSketchPacket:
        decodeSSketchPacket(data);
        break;
    case CommonStructs::MessageType::Debuff:
        _espNow.enqueueGunMessage(data, size);
        sendHealthArmorReport();
        break;
    default:
        break;
    }
}

void GameManager::isGunConnected()
{
    _espNow.isGunPaired();
}

void GameManager::runningIRRecv()
{
    _irrecv.irReceives();
}

void GameManager::espnowHandlingPeer()
{
    _pong.onTimeOut();
}

void GameManager::sendMessageEspNowLoop()
{
    _espNow.sendMessageLoop();
}

void GameManager::runEffects()
{
    _effect.runEffects(_playerData.stats.health, _playerData.stats.armorIncrease,
                       _effect.returnHealReductionValue(), _playerData.stats.armorDecrease,
                       _playerData.stats.extraDamageRecv, _playerData.stats.bonusDamageVulnerability);
}