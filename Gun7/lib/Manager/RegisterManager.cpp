#include "GameManager.h"

void GameManager::addPlayerToMap(uint8_t id, uint8_t teamId, const uint8_t *GunMacAddr, const uint8_t *VestMacAddr)
{
    uint8_t *gunMacCopy = new uint8_t[6];
    uint8_t *vestMacCopy = new uint8_t[6];
    memcpy(gunMacCopy, GunMacAddr, 6);
    memcpy(vestMacCopy, VestMacAddr, 6);

    // Insert player into the map
    _playerMap[id] = std::make_tuple(teamId, GunMacAddr, VestMacAddr);
    _playerMap[id] = std::make_tuple(teamId, gunMacCopy, vestMacCopy);
}

void GameManager::teamRegistering(const JsonDocument &json)
{
    const JsonArrayConst playersArray = json["data"].as<JsonArrayConst>();
    for (const auto &player : playersArray)
    {
        const uint8_t id = player["player_id"];
        const uint8_t team = player["team_id"];
        const char *GunMacString = player["gun_mac_address"];
        const char *VestMacString = player["vest_mac_address"];

        uint8_t gunMacAddr[6];
        uint8_t vestMacAddr[6];

        if (sscanf(GunMacString, "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",
                   &gunMacAddr[0], &gunMacAddr[1], &gunMacAddr[2],
                   &gunMacAddr[3], &gunMacAddr[4], &gunMacAddr[5]) == 6 &&
            sscanf(VestMacString, "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",
                   &vestMacAddr[0], &vestMacAddr[1], &vestMacAddr[2],
                   &vestMacAddr[3], &vestMacAddr[4], &vestMacAddr[5]) == 6)
            addPlayerToMap(id, team, gunMacAddr, vestMacAddr);

        // Send the mapping of each player iteration to the vest
        sendMappingPlayerToVest(id, team, gunMacAddr, vestMacAddr);
    }
    // Print the player map, debugging purposes
    // printPlayerMap();

    // Registering self at once after all players are registered
    selfRegistering();
}

void GameManager::selfRegistering()
{
    for (const auto &entry : _playerMap)
    {
        if (memcmp(_playerData.netCreds._gunMacInt, std::get<1>(entry.second), 6) == 0 &&
            memcmp(_playerData.netCreds.peerInfo.peer_addr, std::get<2>(entry.second), 6) == 0)
        {
            _playerData.matchCreds.id = entry.first;
            _playerData.matchCreds.teamId = std::get<0>(entry.second);

            Serial.print("My ID: ");
            Serial.print(_playerData.matchCreds.id);
            Serial.print(", My Team ID: ");
            Serial.print(_playerData.matchCreds.teamId);
            Serial.print(", Gun MAC: ");
            printMacAddress(_playerData.netCreds._gunMacInt);
            Serial.print(", Vest MAC: ");
            printMacAddress(_playerData.netCreds.peerInfo.peer_addr);
            Serial.println();
        }
    }
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

        if (memcmp(_playerData.netCreds._gunMacInt, gunMac, 6) == 0 &&
            memcmp(_playerData.netCreds.peerInfo.peer_addr, vestMac, 6) == 0)
        {
            Serial.printf("Skipping self registration with ID: %d\n", playerID);
            continue;
        }

        // Add gun MAC as a peer
        esp_now_peer_info_t peerInfo;
        memset(&peerInfo, 0, sizeof(peerInfo));
        memcpy(peerInfo.peer_addr, vestMac, 6);
        peerInfo.channel = _playerData.netCreds.peerInfo.channel;
        peerInfo.encrypt = false;

        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.printf("Failed to add peer with ID: %d\n", entry.first);
        }
        else
        {
            Serial.printf("Successfully added peer with ID: %d", entry.first);
            Serial.print(", With MAC: ");
            printMacAddress(vestMac);
            Serial.println();
        }
    }
}