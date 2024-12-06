// Vest
#include "EspNow.h"

void EspNow::peerRegister(const uint8_t *mac, uint8_t channel)
{
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

    memcpy(_playerData->netCreds.peerInfo.peer_addr, mac, 6);
    _playerData->netCreds.peerInfo.channel = channel;
    _playerData->netCreds.peerInfo.encrypt = false;
    if (esp_now_add_peer(&_playerData->netCreds.peerInfo) == ESP_OK)
    {
        _pong->start();
        _pong->sendPong();
    }
    else
        Serial.println("Failed to add peer");
}

void EspNow::enqueuePlayerMessage(const uint8_t *mac, const uint8_t *data, size_t len)
{
    std::vector<uint8_t> macVec(mac, mac + 6); // Assuming MAC address is 6 bytes
    std::vector<uint8_t> dataVec(data, data + len);
    _playerMessageQueue.push(std::make_pair(std::move(macVec), std::make_pair(std::move(dataVec), len)));
}

void EspNow::enqueueGunMessage(const uint8_t *data, size_t len)
{
    // Convert message to hex string format
    std::string hexMessage = messageToHexString(data, len);

    // If this is the first message in a new batch, set the start time
    if (_currentGunBatch.empty())
    {
        _gunBatchStartTime = millis();
    }

    // If adding this message would exceed MAX_BATCH_SIZE, process current batch
    if (_currentGunBatch.length() + hexMessage.length() > MAX_BATCH_SIZE)
    {
        processBatchedGunMessages();
    }

    // Add to current batch
    _currentGunBatch += hexMessage;
}

void EspNow::processBatchedGunMessages()
{
    if (!_currentGunBatch.empty())
    {
        sendBatchedMessage(_currentGunBatch, _playerData->netCreds.peerInfo.peer_addr);
        _currentGunBatch.clear();
        _gunBatchStartTime = millis();
    }
}

void EspNow::processMessageBatches()
{
    // Process gun message batch if timeout reached or max size reached
    if (!_currentGunBatch.empty() &&
        (millis() - _gunBatchStartTime >= BATCH_TIMEOUT_MS ||
         _currentGunBatch.length() >= MAX_BATCH_SIZE))
    {
        processBatchedGunMessages();
    }

    // Handle normal queue processing for player messages
    sendToEspNowPlayer();
}

std::string EspNow::messageToHexString(const uint8_t *data, size_t len)
{
    static const char hex_chars[] = "0123456789ABCDEF";
    std::string hexString;
    hexString.reserve(len * 2);

    for (size_t i = 0; i < len; ++i)
    {
        hexString += hex_chars[(data[i] >> 4) & 0x0F];
        hexString += hex_chars[data[i] & 0x0F];
    }

    return hexString;
}

void EspNow::sendBatchedMessage(const std::string &batch, const uint8_t *targetMac)
{
    // Convert hex string back to bytes
    std::vector<uint8_t> binaryData;
    binaryData.reserve(batch.length() / 2);

    for (size_t i = 0; i < batch.length(); i += 2)
    {
        uint8_t highNibble = (batch[i] >= 'A') ? (batch[i] - 'A' + 10) : (batch[i] - '0');
        uint8_t lowNibble = (batch[i + 1] >= 'A') ? (batch[i + 1] - 'A' + 10) : (batch[i + 1] - '0');
        binaryData.push_back((highNibble << 4) | lowNibble);
    }

    // Send the batched message
    esp_now_send(targetMac, binaryData.data(), binaryData.size());
}

// Update sendMessageLoop to use the new batching system
void EspNow::sendMessageLoop()
{
    if (millis() - _lastEspNowSendTime >= 3)
    {
        processMessageBatches();
        _lastEspNowSendTime = millis();
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

        // printout the top of the queue
        Serial.print("Sending to: ");
        for (int i = 0; i < 6; i++)
        {
            Serial.print(mac[i], HEX);
            Serial.print(" ");
        }
        Serial.print("Data: ");
        for (int i = 0; i < length; i++)
        {
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }

        _playerMessageQueue.pop();
    }
}

void EspNow::clearQueue(bool gunQueue, bool playerQueue)
{
    if (gunQueue)
    {
        while (!_gunMessageQueue.empty())
            _gunMessageQueue.pop();
    }

    if (playerQueue)
    {
        while (!_playerMessageQueue.empty())
            _playerMessageQueue.pop();
    }
}

const uint8_t *EspNow::getVestMacInt()
{
    esp_efuse_mac_get_default(_playerData->netCreds._mac_address_holder);
    return _playerData->netCreds._mac_address_holder;
}

bool EspNow::isGunPaired() const
{
    return esp_now_is_peer_exist(_playerData->netCreds.peerInfo.peer_addr);
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