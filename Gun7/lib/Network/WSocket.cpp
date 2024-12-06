#include "WSocket.h"

WSocket *WSocket::_instance = nullptr;

WSocket::WSocket(Communication *comm)
    : _communication(comm), _server(81), _ws("/ws"), _isClientConnected(false), _activeClientId(0), _batchStartTime(0)
{
    _instance = this;
}

void WSocket::initWebSocket()
{
    _ws.onEvent(onWebSocketEvent);
    _server.addHandler(&_ws);
}

void WSocket::begin()
{
    initWebSocket();
    _server.begin();
    Serial.println("WebSocket server started on ws://" + WiFi.localIP().toString() + ":81/ws");
    _currentBatch.reserve(MAX_BATCH_SIZE);
}

void WSocket::sendJsonToWebsocket(const JsonDocument &jsonMessage)
{
    if (!_isClientConnected)
        return;

    char buffer[250];
    size_t len = serializeJson(jsonMessage, buffer);
    _ws.text(_activeClientId, buffer, len);
}

void WSocket::sendBinaryToWebsocket(const uint8_t *data, size_t len)
{
    if (!_isClientConnected || len == 0)
        return;

    // Quick size check first
    for (const auto &lastMsg : _lastMessages)
    {
        if (lastMsg.size() == len)
        {
            // Only do memcmp if sizes match
            if (memcmp(data, lastMsg.data(), len) == 0)
            {
                Serial.println("Duplicate message detected");
                return; // Duplicate found
            }
        }
    }

    // Store new message
    _lastMessages[_currentMsgIndex].assign(data, data + len);
    _currentMsgIndex = (_currentMsgIndex + 1) % NUM_LAST_MSGS;

    // Optimized hex conversion using preallocated buffer
    size_t hexLen = 0;
    const size_t maxBytes = (BUFFER_SIZE - 2) / 2; // Account for 2 hex chars per byte + delimiter
    const size_t bytesToProcess = std::min(len, maxBytes);

    for (size_t i = 0; i < bytesToProcess; ++i)
    {
        _hexBuffer[hexLen++] = HEX_CHARS[(data[i] >> 4) & 0x0F];
        _hexBuffer[hexLen++] = HEX_CHARS[data[i] & 0x0F];
    }
    _hexBuffer[hexLen++] = '|';

    // Batch management with pre-checks
    const uint32_t currentTime = millis();
    if (_currentBatch.empty())
    {
        _batchStartTime = currentTime;
    }

    // If adding this message would exceed MAX_BATCH_SIZE, send current batch
    if (_currentBatch.length() + hexLen > MAX_BATCH_SIZE)
    {
        if (!_currentBatch.empty())
        {
            _ws.text(_activeClientId, _currentBatch.c_str(), _currentBatch.length());
            _currentBatch.clear();
            _batchStartTime = currentTime;
        }
    }

    _currentBatch.append(_hexBuffer, hexLen);
}

void WSocket::sendBinaryMessageToWebocketLoop()
{
    if (!_currentBatch.empty() &&
        (millis() - _batchStartTime >= BATCH_TIMEOUT_MS ||
         _currentBatch.length() >= MAX_BATCH_SIZE))
    {
        _ws.text(_activeClientId, _currentBatch.c_str(), _currentBatch.length());
        _currentBatch.clear();
        _batchStartTime = millis();
    }
}

bool WSocket::isPhoneConnected() const
{
    return _isClientConnected;
}

void WSocket::handleNewConnection(AsyncWebSocketClient *client)
{
    _isClientConnected = true;
    _activeClientId = client->id();
    Serial.printf("WebSocket client connected. ID: %u\n", _activeClientId);

    JsonDocument jsonMessage;
    jsonMessage["key"] = "submit_mac";
    _communication->notifyJson(jsonMessage);
}

void WSocket::handleDisconnection()
{
    _isClientConnected = false;
    _activeClientId = 0;
    _ws.cleanupClients();
    Serial.println("WebSocket client disconnected");
}

void WSocket::handleIncomingData(uint8_t *data, size_t len)
{
    DeserializationError error = deserializeJson(_jsonMessage, data);
    if (!error)
    {
        _communication->notifyJson(_jsonMessage);
        _ws.text(_activeClientId, (const char *)data, len);
    }
}

void WSocket::onWebSocketEvent(AsyncWebSocket *server,
                               AsyncWebSocketClient *client,
                               AwsEventType type,
                               void *arg,
                               uint8_t *data,
                               size_t len)
{
    WSocket *self = WSocket::_instance;

    switch (type)
    {
    case WS_EVT_CONNECT:
        if (!self->_isClientConnected)
        {
            self->handleNewConnection(client);
        }
        else
        {
            client->close(1000, "Only one client allowed");
        }
        self->_ws.cleanupClients();
        break;

    case WS_EVT_DISCONNECT:
        if (client->id() == self->_activeClientId)
        {
            self->handleDisconnection();
        }
        break;

    case WS_EVT_DATA:
        if (client->id() == self->_activeClientId && len > 2 &&
            data[0] == '{' && data[len - 1] == '}')
        {
            self->handleIncomingData(data, len);
        }
        break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}