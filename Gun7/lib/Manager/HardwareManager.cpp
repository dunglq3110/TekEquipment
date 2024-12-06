#include "GameManager.h"

void GameManager::shot()
{
    if (!_networkFacade.isVestPaired())
    {
        _infrared.pair(_playerData.netPairingCreds._gunMacPairingLong);
        return;
    }

    if (_playerData.state.isReloading || _playerData.state.playerIsDead)
    {
        return;
    }

    if (_playerData.state.playerisUsingSSketch && _playerData.stats.curSSketchBullet > 0)
    {
        _playerData.stats.curSSketchBullet--;
    }

    else if (!_playerData.state.playerisUsingSSketch && _playerData.stats.curBullet > 0)
    {
        _playerData.stats.curBullet--;
    }

    else
    {
        return;
    }

    _infrared.shot(_playerData.matchCreds.id);
    sendBulletReport();
    updateBulletDisplay();
}

void GameManager::reload()
{
    if (_playerData.state.playerIsDead || _playerData.state.isReloading || _playerData.state.playerisUsingSSketch ||
        _playerData.stats.curBullet == _playerData.stats.maxBullet)
    {
        return;
    }

    else if (_flowControl.isValidToPlay /*&& digitalRead(26) == HIGH*/)
    {
        _playerData.state.isReloading = true;
        _playerData.state.reloadStartTime = millis();
    }
}

void GameManager::switchBullet()
{
    if (_playerData.state.playerIsDead || _playerData.state.isReloading)
    {
        return;
    }

    if (_currentMode == GameStructs::NORMAL)
    {
        if (_playerData.stats.playerHasHeal)
        {
            _currentMode = GameStructs::HEALING;
            _playerData.state.playerIsUsingHeal = true;
            _playerData.state.playerisUsingSSketch = false;
        }
        else if (_playerData.stats.playerHasSSketch)
        {
            _currentMode = GameStructs::SSKETCH;
            _playerData.state.playerIsUsingHeal = false;
            _playerData.state.playerisUsingSSketch = true;
        }
    }
    else if (_currentMode == GameStructs::HEALING)
    {
        if (_playerData.stats.playerHasSSketch)
        {
            _currentMode = GameStructs::SSKETCH;
            _playerData.state.playerIsUsingHeal = false;
            _playerData.state.playerisUsingSSketch = true;
        }
        else
        {
            _currentMode = GameStructs::NORMAL;
            _playerData.state.playerIsUsingHeal = false;
            _playerData.state.playerisUsingSSketch = false;
        }
    }
    else if (_currentMode == GameStructs::SSKETCH)
    {
        _currentMode = GameStructs::NORMAL;
        _playerData.state.playerIsUsingHeal = false;
        _playerData.state.playerisUsingSSketch = false;
    }
    Serial.print("Currently using: ");
    Serial.println(_currentMode);

    updateBulletDisplay();
}

void GameManager::checkReloadStatus()
{
    if (_playerData.state.isReloading && millis() - _playerData.state.reloadStartTime >= _playerData.stats.bulletTime)
    {
        _playerData.stats.curBullet = _playerData.stats.maxBullet;
        _playerData.state.isReloading = false;
        sendBulletReport();
        updateBulletDisplay();
    }

    if (_playerData.stats.curSSketchBullet < _playerData.stats.maxSSketchBullet &&
        millis() - _playerData.state.currentSSketchRegen >= _playerData.stats.sSketchBulletTime)
    {
        _playerData.state.currentSSketchRegen = millis();
        _playerData.stats.curSSketchBullet++;
        sendBulletReport();
        updateBulletDisplay();
    }
}

void GameManager::activateButtons()
{
    if (!_networkFacade.isVestPaired())
        _shootButton.click();
    if (_flowControl.isValidToPlay)
    {
        _shootButton.click();
        _reloadButton.click();
        _switchButton.click();
    }
}

void GameManager::setBattlePhase(bool state)
{
    _flowControl.isBattlePhase = state;
}

void GameManager::setValidToPlay()
{
    if (!WiFi.isConnected())
    {
        _flowControl.isValidToPlay = false;
        _flowControl.isDrawBattleFirstTime = false;
        _display.setWiFiStatus(false, false, _playerData.netPairingCreds._gunMacStr.c_str());
        return;
    }

    if (_playerData.netPairingCreds._channel > 11)
    {
        _flowControl.isValidToPlay = false;
        _flowControl.isDrawBattleFirstTime = false;
        _display.setWiFiStatus(true, false, _playerData.netPairingCreds._gunMacStr.c_str());
        return;
    }

    if (_flowControl.isBattlePhase && _networkFacade.isVestPaired() && _networkFacade.isPhoneConnected())
    {
        _flowControl.isValidToPlay = true;
        if (!_flowControl.isDrawBattleFirstTime)
        {
            _display.drawBorder(1);
            updateBulletDisplay();
            updateInformationDisplay();
            updateDebuffDisplay();
            forceUpdateBulletDisplay();
            forceUpdateInformationDisplay();
            forceUpdateDebuffDisplay();
            _flowControl.isDrawBattleFirstTime = true;
        }
        return;
    }

    _display.setConnectionsState(_networkFacade.isVestPaired(), _networkFacade.isPhoneConnected(),
                                 _flowControl.isBattlePhase, WiFi.localIP().toString().c_str(),
                                 std::to_string(_playerData.netPairingCreds._channel));
    if (_flowControl.isDrawBattleFirstTime)
        _display.drawConnectionsState();
    _flowControl.isValidToPlay = false;
    _flowControl.isDrawBattleFirstTime = false;
}

void GameManager::updateBulletDisplay()
{
    _display.setBulletSection(_playerData.stats.curBullet, _playerData.stats.maxBullet,
                              _playerData.stats.curSSketchBullet, _playerData.stats.maxSSketchBullet,
                              _playerData.stats.playerHasHeal, _playerData.state.playerIsUsingHeal,
                              _playerData.stats.playerHasSSketch, _playerData.state.playerisUsingSSketch);
}

void GameManager::updateInformationDisplay()
{
    _display.setInformationSection(_playerData.stats.health, _playerData.stats.armor,
                                   _playerData.stats.damage, _playerData.stats.heal);
}

void GameManager::updateDebuffDisplay()
{
    _display.setDebuffSection(_effects.debuffDisplayStorage.fireLvl, _effects.debuffDisplayStorage.fireDur,
                              _effects.debuffDisplayStorage.poisonLvl, _effects.debuffDisplayStorage.poisonDur,
                              _effects.debuffDisplayStorage.deHealLvl, _effects.debuffDisplayStorage.deHealDur,
                              _effects.debuffDisplayStorage.exposedLvl, _effects.debuffDisplayStorage.exposedDur,
                              _effects.debuffDisplayStorage.shatteredLvl, _effects.debuffDisplayStorage.shatteredDur,
                              0, 0,
                              0, 0,
                              0, 0);
}

void GameManager::forceUpdateBulletDisplay()
{
    _display.drawBulletSection();
}

void GameManager::forceUpdateInformationDisplay()
{
    _display.drawInformationSection();
}

void GameManager::forceUpdateDebuffDisplay()
{
    _display.drawDebuffSection();
}