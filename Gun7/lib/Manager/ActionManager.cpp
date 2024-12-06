#include "GameManager.h"

void GameManager::setReadyForBattle(const JsonDocument &json)
{
    if (reportVestConnected())
    {
        if (_playerData.state.playerIsDead)
        {
            setRevival();
        }
        else
            setNotDead();

        setBattlePhase(true);
        _playerData.setHealthArmor(json["for_vest"]);
        _playerData.setStats(json["for_gun"]);
        _effects.setDebuffs(json["for_gun"]);
        sendVestStatPacket(json["for_vest"]);
        registerPlayersAsPeers();

        sendUpgradeChipReport();

        updateBulletDisplay();
        updateInformationDisplay();
        _display.drawBorder(1);
    }
}

void GameManager::setStopBattling()
{
    setBattlePhase(false);
    _playerData.resetStats();
    _display.fullDisplayReset();
    _effects.resetEffects();
}

void GameManager::setIsDead()
{
    Serial.println("Setting dead");
    _playerData.state.playerIsDead = true;
    _playerData.preserveStats = _playerData.stats;
    _playerData.stats = {};
    _deadLed.on();
    _display.setIsDead(true);
}

void GameManager::setRevival()
{
    Serial.println("Setting revival");
    _playerData.state.playerIsDead = false;
    _playerData.stats = _playerData.preserveStats;
    _playerData.preserveStats = {};
    _deadLed.off();
    _display.setIsDead(false);
}

void GameManager::setNotDead()
{
    Serial.println("Setting not dead");
    _playerData.state.playerIsDead = false;
    _deadLed.off();
    _display.setIsDead(false);
}