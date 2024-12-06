#ifndef WSOCKET_H
#define WSOCKET_H

#include <WiFi.h>
#include <queue>
#include <AsyncTCP.h>
#include <hashtable.h>
#include "Communication.h"
#include <ESPAsyncWebServer.h>

class WSocket
{
public:
    explicit WSocket(Communication *comm);
    // ~WSocket() = default;

    void begin();
    void sendJsonToWebsocket(const JsonDocument &jsonMessage);
    void sendBinaryToWebsocket(const uint8_t *data, size_t len);
    void sendBinaryMessageToWebocketLoop();
    bool isPhoneConnected() const;

private:
    static constexpr size_t BUFFER_SIZE = 512;
    static constexpr size_t MAX_BATCH_SIZE = 1024;
    static constexpr uint32_t BATCH_TIMEOUT_MS = 100;
    static constexpr const char *HEX_CHARS = "0123456789ABCDEF";

    // Report duplicate check
    static constexpr size_t NUM_LAST_MSGS = 2; // Store last 2 messages
    std::array<std::vector<uint8_t>, NUM_LAST_MSGS> _lastMessages;
    size_t _currentMsgIndex = 0;

    // Static instance for callback handling
    static WSocket *_instance;
    void initWebSocket();
    static void onWebSocketEvent(AsyncWebSocket *server,
                                 AsyncWebSocketClient *client,
                                 AwsEventType type,
                                 void *arg,
                                 uint8_t *data,
                                 size_t len);

    // Event handlers
    void handleNewConnection(AsyncWebSocketClient *client);
    void handleDisconnection();
    void handleIncomingData(uint8_t *data, size_t len);

    // Member variables
    Communication *_communication;
    AsyncWebServer _server;
    AsyncWebSocket _ws;
    bool _isClientConnected;
    uint32_t _activeClientId;
    uint32_t _batchStartTime;

    // Buffers and data handling
    char _hexBuffer[BUFFER_SIZE];
    std::string _currentBatch;
    std::vector<uint8_t> _lastDataInQueue;
    JsonDocument _jsonMessage;
};

#endif