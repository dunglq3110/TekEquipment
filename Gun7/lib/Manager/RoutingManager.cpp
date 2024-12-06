#include "GameManager.h"

GameManager::GameManager()
    : _shootButton(13, std::bind(&GameManager::shot, this), std::bind(&GameManager::doNothing, this)),
      _reloadButton(32, std::bind(&GameManager::reload, this), std::bind(&GameManager::doNothing, this)),
      _switchButton(33, std::bind(&GameManager::switchBullet, this), std::bind(&GameManager::doNothing, this)),
      _infrared(14),
      _deadLed(25),
      _ping(&_playerData, &_communication),
      _server(&_communication, &_playerData, &_ping),
      _socket(&_communication),
      _espNow(&_communication, &_playerData, &_ping),
      _networkFacade(&_communication, &_server, &_espNow, &_socket, &_playerData, &_ping)
{
    _communication.attach(this);
    _currentMode = GameStructs::NORMAL;
}

void GameManager::init()
{
    _infrared.init();
    _display.init();
    _upgradeChip.init();
    _display.drawWiFiFinding();
    _networkFacade.initPreGame();
}

void GameManager::updateMesg(const std::string &message)
{
}

void GameManager::updateJson(const JsonDocument &json)
{
    if (json["key"] == "players_registering")
    {
        if (reportVestConnected())
        {
            teamRegistering(json);
        }
    }

    else if (json["key"] == "start_battle")
    {
        setReadyForBattle(json);
    }

    else if (json["key"] == "end_battle")
    {
        }

    else if (json["key"] == "submit_mac")
    {
        sendMacSubmission();
    }
}

void GameManager::updateBinary(const uint8_t *data, size_t size)
{
    _header.type = static_cast<CommonStructs::MessageType>(data[0] & 0x0F);
    analyzeMessage(_header.type, data);
}

void GameManager::doNothing()
{
}

void GameManager::analyzeMessage(CommonStructs::MessageType type, const uint8_t *buffer)
{
    if (_header.type != CommonStructs::MessageType::Pong)
    {
        Serial.print("Received header via binary message notify: ");
        Serial.print(_enumIndexes[static_cast<int>(_header.type)]);
        Serial.print("\tLength: ");
        Serial.println(sizeof(buffer));
    }

    switch (type)
    {
    case CommonStructs::MessageType::Pong:
        _networkFacade.onPingReceived();
        break;

    case CommonStructs::MessageType::ConfirmPacket:
        if (!_playerData.state.playerIsDead)
            responseToConfirmPackets(const_cast<uint8_t *>(buffer));
        break;

    case CommonStructs::MessageType::Debuff:
        _debuffHeader.header.type = static_cast<CommonStructs::MessageType>(buffer[0] & 0x0F);
        _debuffHeader.debuffType = static_cast<CommonStructs::DebuffType>(buffer[1] & 0x0F);
        if (deserialize(_debuffHeader, buffer))
            analyzeAndDisplayDebuffs(_debuffHeader, buffer);
        break;

    case CommonStructs::MessageType::HealthArmorReport:
        if (deserialize(_healthArmorReport, buffer))
        {
            _playerData.stats.health = _healthArmorReport.health;
            _playerData.stats.armor = _healthArmorReport.armor;
            updateInformationDisplay();
            _networkFacade.forwardBinaryToWSocket(buffer, sizeof(GameStructs::HealthArmorReport));
        }
        break;

    case CommonStructs::MessageType::DamageReport:
        if (deserialize(_damageReport, buffer))
            _networkFacade.forwardBinaryToWSocket(buffer, sizeof(_damageReport));
        break;

    case CommonStructs::MessageType::HealingReport:
        if (deserialize(_healingReport, buffer))
            _networkFacade.forwardBinaryToWSocket(buffer, sizeof(_healingReport));
        break;

    case CommonStructs::MessageType::SSketchReport:
        if (deserialize(_sSketchReport, buffer))
            _networkFacade.forwardBinaryToWSocket(buffer, sizeof(_sSketchReport));
        break;

    case CommonStructs::MessageType::IsDeadReport:
        setIsDead();
        break;
    }
}

void GameManager::isVestConnected()
{
    _networkFacade.isVestPaired();
}

void GameManager::isPhoneConnected()
{
    _networkFacade.isPhoneConnected();
}

void GameManager::wServerHandlingClient()
{
    _networkFacade.wServerHandlingClient();
}

void GameManager::espNowHandlingPeer()
{
    _networkFacade.espnowHandlingPeer();
}

void GameManager::sendMessageEspNowLoop()
{
    _networkFacade.sendMessageEspNowLoop();
}

void GameManager::sendBinaryToWSocketLoop()
{
    _networkFacade.sendBinaryToWSocketLoop();
}