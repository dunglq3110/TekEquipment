#include "PingPong.h"

PingPong::PingPong(PlayerData *data) : _playerData(data)
{
}

void PingPong::start()
{
    _lastPongTime = millis();
    _isActive = true;
}

void PingPong::stop()
{
    _isActive = false;
}

void PingPong::onResponseReceived() // Calls in routing manager
{
    _lastPongTime = millis();
    sendPong();
}

void PingPong::onTimeOut() // Put in loop
{
    if (isTimeout() && _isActive)
    {
        stop();
        esp_now_del_peer(_playerData->netCreds.peerInfo.peer_addr);
        memset(_playerData->netCreds.peerInfo.peer_addr, 0, 6);
        Serial.println("++++++++++++++++++++++++++++++++++++++++");
        Serial.println("Peer timed out, deleting peer");
        Serial.println("++++++++++++++++++++++++++++++++++++++++");
    }
}

bool PingPong::isTimeout() const
{
    return millis() - _lastPongTime > _pongTimeoutInterval;
}

void PingPong::sendPong()
{
    uint8_t buffer[sizeof(CommonStructs::MessageHeader)];
    CommonStructs::MessageHeader header = {CommonStructs::MessageType::Pong};
    *buffer = static_cast<uint8_t>(header.type);
    esp_now_send(_playerData->netCreds.peerInfo.peer_addr, buffer, sizeof(buffer));
}
