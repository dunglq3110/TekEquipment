#ifndef ESPNOW_H
#define ESPNOW_H

#include "Communication.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include "GameStructs.h"
#include "PlayerData.h"
#include "PingPong.h"
#include <queue>

class EspNow
{
private:
    Communication *_communication;
    PlayerData *_playerData;
    PingPong *_pong;
    JsonDocument _jsonSent;
    static EspNow *_instance;

    unsigned long long _lastEspNowSendTime;

    // Message queue, avoiding maximum bandwidth when sending at once
    std::queue<std::pair<std::vector<uint8_t>, size_t>> _gunMessageQueue;
    std::queue<std::pair<std::vector<uint8_t>, std::pair<std::vector<uint8_t>, size_t>>> _playerMessageQueue;

    /* Callback functions */
    void onDataRecv(const uint8_t *mac, const uint8_t *data, int len);
    void onDataSent(const uint8_t *mac, esp_now_send_status_t status);

public:
    /* Static related functions, need for the Esp-Now library to work */
    explicit EspNow(Communication *comm, PlayerData *playerData, PingPong *pong);
    static void onDataRecvStatic(const uint8_t *mac, const uint8_t *data, int len);
    static void onDataSentStatic(const uint8_t *mac, esp_now_send_status_t status);

    // Batching system for gun messages
    std::string _currentGunBatch;
    unsigned long _gunBatchStartTime;
    static constexpr size_t BATCH_TIMEOUT_MS = 100; // Same as WebSocket
    static constexpr size_t MAX_BATCH_SIZE = 250;   // ESP-NOW packet limit

    void processMessageBatches(); // New method to handle batch processing

    // Helper methods for batching
    void processBatchedGunMessages();
    void processBatchedPlayerMessages();
    void sendBatchedMessage(const std::string &batch, const uint8_t *targetMac);
    std::string messageToHexString(const uint8_t *data, size_t len);

    /* Central functions */
    void init();
    void peerRegister(const uint8_t *mac, uint8_t channel);
    void sendToEspNowGun();
    void sendToEspNowPlayer();
    void sendMessageLoop();
    void enqueuePlayerMessage(const uint8_t *mac, const uint8_t *data, size_t len);
    void enqueueGunMessage(const uint8_t *data, size_t len);
    void clearQueue(bool gunQueue, bool playerQueue);

    /* Flow control functions */
    bool isGunPaired() const;

    /* Helper Fuctions */
    const uint8_t *getVestMacInt();
};

#endif