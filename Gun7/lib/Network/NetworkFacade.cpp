#include "NetworkFacade.h"

NetworkFacade::NetworkFacade(Communication *comm = nullptr, WServer *ser = nullptr, EspNow *now = nullptr, WSocket *soc = nullptr, PlayerData *data = nullptr, PingPong *ping = nullptr)
{
    this->_communication = comm ?: new Communication;
    this->_playerData = data ?: new PlayerData;
    this->_ping = ping ?: new PingPong(_playerData, _communication);
    this->_espNow = now ?: new EspNow(_communication, _playerData, _ping);
    this->_wServer = ser ?: new WServer(_communication, _playerData, _ping);
    this->_wSocket = soc ?: new WSocket(_communication);
}

void NetworkFacade::initPreGame()
{
    _wServer->init();
    _wServer->connect();

    _espNow->init();
    _espNow->setChannel(13);
    _espNow->setCredentials();

    if (WiFi.status() == WL_CONNECTED)
    {
        uint8_t chan = _wServer->findWiFiChannel();
        if (chan != 0)
        {
            _espNow->setChannel(chan);
            _espNow->setCredentials();
            _wSocket->begin();
            Serial.print("Pairing MAC in heximal: ");
            Serial.println((_playerData->netPairingCreds._gunMacPairingStr).c_str());
            Serial.print("Pairing MAC in decimal: ");
            Serial.println(_playerData->netPairingCreds._gunMacPairingLong);
        }
        else
            Serial.println("The channel is 0?");
    }
    else
        Serial.println("There is no such WiFi name!");
}

bool NetworkFacade::isPhoneConnected()
{
    return _wSocket->isPhoneConnected();
}

bool NetworkFacade::isVestPaired()
{
    return _espNow->isVestPaired();
}

void NetworkFacade::onPingReceived()
{
    _ping->onResponseReceived();
}

void NetworkFacade::forwardJsonToWSocket(const JsonDocument &json)
{
    _wSocket->sendJsonToWebsocket(json);
}

void NetworkFacade::forwardBinaryToWSocket(const uint8_t *data, size_t len)
{
    _wSocket->sendBinaryToWebsocket(data, len);
}

// Runs on system loop
void NetworkFacade::sendMessageEspNowLoop()
{
    _espNow->sendMessageLoop();
}

// Runs on system loop
void NetworkFacade::sendBinaryToWSocketLoop()
{
    _wSocket->sendBinaryMessageToWebocketLoop();
}

void NetworkFacade::enqueueMessageToEspNowVest(const uint8_t *data, size_t len)
{
    _espNow->enqueueVestMessage(data, len);
}

void NetworkFacade::enqueueMessageToEspNowPlayer(const uint8_t *data, const uint8_t *mac, size_t len)
{
    _espNow->enqueuePlayerMessage(mac, data, len);
}

// Runs on system loop
void NetworkFacade::wServerHandlingClient()
{
    _wServer->handleClient();
}

// Runs on system loop
void NetworkFacade::espnowHandlingPeer()
{
    _ping->update();
    _ping->onTimeOut();
}