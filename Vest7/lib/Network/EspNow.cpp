#include "EspNow.h"

EspNow *EspNow::_instance = nullptr;

EspNow::EspNow(Communication *comm, PlayerData *data, PingPong *pong) : _communication(comm), _playerData(data), _pong(pong)
{
    _instance = this;
}

void EspNow::init()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    esp_wifi_set_promiscuous(true);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(esp_now_recv_cb_t(onDataRecvStatic));
    esp_now_register_send_cb(esp_now_send_cb_t(onDataSentStatic));

    // Set vest/self mac here
    _playerData->netCreds.vestMacint = getVestMacInt();
}

void EspNow::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void EspNow::onDataRecv(const uint8_t *mac, const uint8_t *data, int len)
{
    DeserializationError error = deserializeJson(_jsonSent, data, len);
    if (!error)
    {
        _communication->notifyJson(_jsonSent);
    }
    else
    {
        _communication->notifyBinary(data, len, 0);
    }
}