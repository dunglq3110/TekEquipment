#include "GameManager.h"

void GameManager::responseToConfirmPackets(const uint8_t *buffer)
{
    if (deserialize(_confirmPacket, buffer) /*&& _flowControl.isValidToPlay*/)
    {
        uint8_t id = _confirmPacket.id;
        uint8_t teamId = _confirmPacket.teamId;

        Serial.print("Player ID: ");
        Serial.print(id);
        Serial.print(", Team ID: ");
        Serial.print(teamId);

        Serial.print(", My ID: ");
        Serial.print(_playerData.matchCreds.id);
        Serial.print(", My Team ID: ");
        Serial.print(_playerData.matchCreds.teamId);

        Serial.print(", Sending Packet to: ");
        Serial.println(id);

        if (_playerData.state.playerIsUsingHeal)
        {
            if (/*_playerData.matchCreds.teamId == teamId &&*/ _playerData.matchCreds.id != id)
                sendHealingPacket(findMacAddressByIdInMap(id));
        }
        else if (_playerData.state.playerisUsingSSketch)
        {
            if (_playerData.matchCreds.teamId != teamId && _playerData.matchCreds.id != id)
                sendSSketchPacket(findMacAddressByIdInMap(id));
        }
        else
        {
            if (_playerData.matchCreds.teamId != teamId && _playerData.matchCreds.id != id)
                sendDamagePacket(findMacAddressByIdInMap(id));
        }
    }
}

void GameManager::analyzeAndDisplayDebuffs(CommonStructs::DebuffHeader debuffHeader, const uint8_t *buffer)
{
    switch (debuffHeader.debuffType)
    {
    case CommonStructs::DebuffType::Fire:
        if (deserialize(_fireDisplay, buffer))
        {
            _effects.debuffDisplayStorage.fireLvl = _fireDisplay.level;
            _effects.debuffDisplayStorage.fireDur = _fireDisplay.duration;
        }
        break;

    case CommonStructs::DebuffType::Poison:
        if (deserialize(_poisonDisplay, buffer))
        {
            _effects.debuffDisplayStorage.poisonLvl = _poisonDisplay.level;
            _effects.debuffDisplayStorage.poisonDur = _poisonDisplay.duration;
        }
        break;

    case CommonStructs::DebuffType::DeHeal:
        if (deserialize(_deHealDisplay, buffer))
        {
            _effects.debuffDisplayStorage.deHealLvl = _deHealDisplay.level;
            _effects.debuffDisplayStorage.deHealDur = _deHealDisplay.duration;
        }
        break;

    case CommonStructs::DebuffType::Silence:
        if (deserialize(_silenceDisplay, buffer))
        {
            _effects.debuffDisplayStorage.silenceLvl = _silenceDisplay.level;
            _effects.debuffDisplayStorage.silenceDur = _silenceDisplay.duration;
        }
        break;

    case CommonStructs::DebuffType::Exposed:
        if (deserialize(_exposedDisplay, buffer))
        {
            _effects.debuffDisplayStorage.exposedLvl = _exposedDisplay.level;
            _effects.debuffDisplayStorage.exposedDur = _exposedDisplay.duration;
        }
        break;

    case CommonStructs::DebuffType::Shattered:
        if (deserialize(_shatteredDisplay, buffer))
        {
            _effects.debuffDisplayStorage.shatteredLvl = _shatteredDisplay.level;
            _effects.debuffDisplayStorage.shatteredDur = _shatteredDisplay.duration;
        }
        break;
    }

    updateDebuffDisplay();
}
