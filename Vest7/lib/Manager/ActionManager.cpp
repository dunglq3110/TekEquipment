#include "GameManager.h"

bool GameManager::convertMessageToHexAndSplit(const std::string &message, uint8_t *mac, uint8_t &channel)
{
    unsigned long long number;
    try
    {
        number = std::stoull(message);
    }
    catch (const std::exception &e)
    {
        Serial.println("Error: Invalid input string");
        return false;
    }

    char hexString[15];
    snprintf(hexString, sizeof(hexString), "%014llX", number);
    if (strlen(hexString) != 14)
    {
        Serial.println("Error: Hexadecimal string length is not 14 characters.");
        return false;
    }

    std::string hexStr(hexString);
    std::string part1 = hexStr.substr(0, 12); /* 6 bytes for MAC address */
    std::string part2 = hexStr.substr(12);    /* 2 bytes for channel number */

    for (int i = 0; i < 6; ++i)
    {
        std::string byteStr = part1.substr(i * 2, 2);
        mac[i] = (uint8_t)strtoul(byteStr.c_str(), nullptr, 16);
    }
    channel = (uint8_t)strtoul(part2.c_str(), nullptr, 16);

    return true;
}

void GameManager::isPlayerDead()
{
    // if (_playerData.preserveStats.health < _playerData.stats.health && _playerData.state.isGameStarted)
    // {
    //     _playerData.stats.health = 0;
    //     _playerData.preserveStats = _playerData.stats;
    //     _playerData.stats = {};
    //     _playerData.state.playerIsDead = true;

    //     _espNow.clearQueue(true, true);

    //     Serial.println("Player is dead");
    //     sendIsDeadReport();
    // }
    // else
    // {
    //     _playerData.preserveStats.health = _playerData.stats.health;
    //     _playerData.state.playerIsDead = false;
    // }
}