#include "GameManager.h"

const uint8_t *GameManager::findMacAddressByIdInMap(uint64_t id)
{
    auto it = _playerMap.find(id);
    if (it != _playerMap.end())
    {
        const auto &playerInfo = it->second;
        Serial.println("Player for IR response found");
        return std::get<1>(playerInfo);
    }
    else
    {
        Serial.println("Player for IR response not found");
        return nullptr;
    }
}

void GameManager::addPlayerToMap(uint8_t id, uint8_t teamId, const uint8_t *GunMacAddr, const uint8_t *VestMacAddr)
{
    uint8_t *gunMacCopy = new uint8_t[6];
    uint8_t *vestMacCopy = new uint8_t[6];
    memcpy(gunMacCopy, GunMacAddr, 6);
    memcpy(vestMacCopy, VestMacAddr, 6);

    // Insert into the map
    _playerMap[id] = std::make_tuple(teamId, gunMacCopy, vestMacCopy);
}

void GameManager::printPlayerMap()
{
    for (const auto &entry : _playerMap)
    {
        const uint8_t id = entry.first;
        const uint8_t teamId = std::get<0>(entry.second);
        const uint8_t *gunMacAddr = std::get<1>(entry.second);
        const uint8_t *vestMacAddr = std::get<2>(entry.second);

        Serial.print("Player ID: ");
        Serial.print(id);
        Serial.print(", Team ID: ");
        Serial.print(teamId);
        Serial.print(", Gun's MAC Address: ");
        printMacAddress(gunMacAddr);
        Serial.print(", Vest's MAC Address: ");
        printMacAddress(vestMacAddr);
        Serial.println();
    }
}

void GameManager::printMacAddress(const uint8_t *macAddr)
{
    for (int i = 0; i < 6; i++)
    {
        if (i > 0)
            Serial.print(":");
        if (macAddr[i] < 0x10)
            Serial.print("0");
        Serial.print(macAddr[i], HEX);
    }
}

void GameManager::playerRegistering(uint8_t id, uint8_t teamId, const uint8_t *GunMacAddr, const uint8_t *VestMacAddr)
{
    if (memcmp(_playerData.netCreds.peerInfo.peer_addr, GunMacAddr, 6) == 0 && memcmp(_playerData.netCreds.vestMacint, VestMacAddr, 6) == 0)
    {
        // Filled player data with credentials for later usage.
        _playerData.matchCreds.id = id;
        _playerData.matchCreds.teamId = teamId;

        // Establish the ConfirmPacket local instance
        _confirmPacket.header.type = CommonStructs::MessageType::ConfirmPacket;
        _confirmPacket.id = _playerData.matchCreds.id;
        _confirmPacket.teamId = _playerData.matchCreds.teamId;

        // Debug print
        Serial.print("My ID: ");
        Serial.print(_playerData.matchCreds.id);
        Serial.print(", My Team ID: ");
        Serial.print(_playerData.matchCreds.teamId);
        Serial.print(", Gun MAC: ");
        printMacAddress(_playerData.netCreds.peerInfo.peer_addr);
        Serial.print(", Vest MAC: ");
        printMacAddress(_playerData.netCreds.vestMacint);
        Serial.println();
    }
    addPlayerToMap(id, teamId, GunMacAddr, VestMacAddr);
}

void GameManager::registerPlayersAsPeers()
{
    for (const auto &entry : _playerMap)
    {
        uint8_t playerID = entry.first;
        uint8_t teamID;
        const uint8_t *gunMac;
        const uint8_t *vestMac;

        // Unpack the tuple
        std::tie(teamID, gunMac, vestMac) = entry.second;

        if (memcmp(_playerData.netCreds.peerInfo.peer_addr, gunMac, 6) == 0 &&
            memcmp(_playerData.netCreds.vestMacint, vestMac, 6) == 0)
        {
            Serial.printf("Skipping self registration with ID: %d\n", playerID);
            continue;
        }

        // Add gun MAC as a peer
        esp_now_peer_info_t peerInfo;
        memset(&peerInfo, 0, sizeof(peerInfo));
        memcpy(peerInfo.peer_addr, gunMac, 6);
        peerInfo.channel = _playerData.netCreds.peerInfo.channel;
        peerInfo.encrypt = false;

        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.printf("Failed to add peer with ID: %d\n", entry.first);
        }
        else
        {
            Serial.printf("Successfully added peer with ID: %d\n", entry.first);
        }
    }
}