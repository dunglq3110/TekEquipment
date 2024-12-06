#include "EspNow.h"

void EspNow::peerRegister(const uint8_t *mac)
{
    if (!isVestPaired())
    {
        memcpy(_playerData->netCreds.peerInfo.peer_addr, mac, 6);

        if (esp_now_add_peer(&_playerData->netCreds.peerInfo) == ESP_OK)
        {
            _ping->start();
            JsonDocument jsonMessage;
            jsonMessage["key"] = "submit_mac";
            _communication->notifyJson(jsonMessage);
        }
    }
}

void EspNow::sendMessageLoop()
{
    if (millis() - _lastEspNowSendTime >= 2)
    {
        sendToEspNowVest();
        sendToEspNowPlayer();
        _lastEspNowSendTime = millis();
    }
}

void EspNow::enqueuePlayerMessage(const uint8_t *mac, const uint8_t *data, size_t len)
{
    std::vector<uint8_t> macVec(mac, mac + 6); // Assuming MAC address is 6 bytes
    std::vector<uint8_t> dataVec(data, data + len);
    _playerMessageQueue.push(std::make_pair(std::move(macVec), std::make_pair(std::move(dataVec), len)));
}

void EspNow::enqueueVestMessage(const uint8_t *data, size_t len)
{
    _vestMessageQueue.push(std::make_pair(std::vector<uint8_t>(data, data + len), len));
}

void EspNow::sendToEspNowVest()
{
    if (isVestPaired() && !_vestMessageQueue.empty())
    {
        const auto &front = _vestMessageQueue.front();
        const std::vector<uint8_t> &message = front.first;
        size_t length = front.second;
        esp_now_send(_playerData->netCreds.peerInfo.peer_addr, message.data(), length);
        _vestMessageQueue.pop();
    }
}

void EspNow::sendToEspNowPlayer()
{
    if (!_playerMessageQueue.empty())
    {
        const auto &front = _playerMessageQueue.front();
        const std::vector<uint8_t> &mac = front.first;
        const auto &dataPair = front.second;
        const std::vector<uint8_t> &data = dataPair.first;
        size_t length = dataPair.second;
        esp_now_send(mac.data(), data.data(), length);
        _playerMessageQueue.pop();
    }
}

unsigned long long EspNow::getGunMacParingLong() const
{
    return macIntoDecimalLong(getGunMacPairingStr());
}

const uint8_t *EspNow::getGunMacInt()
{
    esp_efuse_mac_get_default(_playerData->netPairingCreds._mac_address_holder);
    return _playerData->netPairingCreds._mac_address_holder;
}

std::string EspNow::getGunMacStr() const
{
    uint8_t mac[6] = {0};
    esp_efuse_mac_get_default(mac);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string(macStr);
}

std::string EspNow::getGunMacPairingStr() const
{
    uint8_t mac[6] = {0};
    esp_efuse_mac_get_default(mac);
    /* 17 (MAC) + 1 (:) + 2 (channel) + 1 (null terminator) */
    char macWithChannel[21];
    snprintf(macWithChannel, sizeof(macWithChannel), "%02X:%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], _playerData->netPairingCreds._channel);
    return std::string(macWithChannel);
};

unsigned long long EspNow::macIntoDecimalLong(const std::string &mac) const
{
    unsigned long long decimal = 0;
    for (int i = 0; i < 7; ++i)
    {
        decimal = (decimal << 8) | std::stoul(mac.substr(i * 3, 2), nullptr, 16);
    }
    return decimal;
}

void EspNow::onDataRecvStatic(const uint8_t *mac, const uint8_t *data, int len)
{
    if (_instance)
        _instance->onDataRecv(mac, data, len);
}

void EspNow::onDataSentStatic(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (_instance)
        _instance->onDataSent(mac_addr, status);
}
