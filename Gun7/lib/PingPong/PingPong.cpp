#include "PingPong.h"

PingPong::PingPong(PlayerData *data, Communication *comm) : _playerData(data), _communication(comm)
{
}

void PingPong::start()
{
    _lastPingTime = millis();
    _isActive = true;
    _receivedResponse = true;
}

void PingPong::stop()
{
    _isActive = false;
    _receivedResponse = false;
}

void PingPong::update() // Put in loop
{
    if (millis() - _lastPingTime > _pingInterval && _isActive && _receivedResponse)
    {
        sendPing();
        _receivedResponse = false;
    }
}

void PingPong::sendPing()
{
    uint8_t buffer[sizeof(CommonStructs::MessageHeader)];
    CommonStructs::MessageHeader header = {CommonStructs::MessageType::Ping};
    *buffer = static_cast<uint8_t>(header.type);
    esp_now_send(_playerData->netCreds.peerInfo.peer_addr, buffer, sizeof(buffer));
}

bool PingPong::isTimeout() const
{
    return millis() - _lastPingTime > _pingTimeoutInterval;
}

void PingPong::onResponseReceived() // Calls in routing manager
{
    _lastPingTime = millis();
    _receivedResponse = true;
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

        JsonDocument jsonMessage;
        jsonMessage["key"] = "submit_mac";
        _communication->notifyJson(jsonMessage);
    }
}