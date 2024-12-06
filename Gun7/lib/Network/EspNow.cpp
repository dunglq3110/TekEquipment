// Gun
#include "EspNow.h"

EspNow *EspNow::_instance = nullptr;

EspNow::EspNow(Communication *comm, PlayerData *data, PingPong *ping) : _communication(comm), _playerData(data), _ping(ping)
{
    _instance = this;
}

void EspNow::setCredentials()
{
    _playerData->netCreds._gunMacInt = getGunMacInt();
    _playerData->netPairingCreds._gunMacStr = getGunMacStr();
    _playerData->netPairingCreds._gunMacPairingStr = getGunMacPairingStr();
    _playerData->netPairingCreds._gunMacPairingLong = getGunMacParingLong();
}

void EspNow::setChannel(uint8_t channel)
{
    _playerData->netPairingCreds._channel = channel;
}

bool EspNow::isVestPaired() const
{
    return esp_now_is_peer_exist(_playerData->netCreds.peerInfo.peer_addr);
}

void EspNow::init()
{
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(_playerData->netPairingCreds._channel, WIFI_SECOND_CHAN_NONE);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(esp_now_recv_cb_t(onDataRecvStatic));
    esp_now_register_send_cb(esp_now_send_cb_t(onDataSentStatic));
}

void EspNow::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void EspNow::onDataRecv(const uint8_t *mac, const uint8_t *data, int len)
{
    // fine for now to register right here and not in the GameFacade.cpp?
    if (!isVestPaired())
    {
        peerRegister(mac);
    }
    else
    {
        DeserializationError error = deserializeJson(_jsonSent, data, len);
        if (!error)
        {
            _communication->notifyJson(_jsonSent);
        }
        else
        {
            Serial.print("Data is binary, Data is: ");
            for (int i = 0; i < len; i++)
            {
                Serial.print(data[i]);
                Serial.print(":");
            }
            Serial.print(", From MAC: ");
            for (int i = 0; i < 6; i++)
            {
                Serial.print(mac[i], HEX);
                Serial.print(":");
            }
            Serial.print(", With length: ");
            Serial.println(len);
            Serial.println();
            processBatchedData(data, len);
        }
    }
}

void EspNow::processBatchedData(const uint8_t *data, size_t len)
{
    if (len > 0)
    {
        uint8_t *data_ptr = const_cast<uint8_t *>(data);
        size_t remaining_data = len;

        CommonStructs::MessageHeader header;
        header.type = static_cast<CommonStructs::MessageType>(data[0] & 0x0F);

        switch (header.type)
        {
        case CommonStructs::MessageType::Pong:
            _communication->notifyBinary(data, 1);
            data_ptr += 1;
            remaining_data -= 1;
            break;
        case CommonStructs::MessageType::ConfirmPacket:
            _communication->notifyBinary(data, sizeof(GameStructs::ConfirmPacket));
            data_ptr += sizeof(GameStructs::ConfirmPacket);
            remaining_data -= sizeof(GameStructs::ConfirmPacket);
            break;
        case CommonStructs::MessageType::Debuff:
            CommonStructs::DebuffHeader debuffHeader;
            debuffHeader.debuffType = static_cast<CommonStructs::DebuffType>(data[1] & 0x0F);
            switch (debuffHeader.debuffType)
            {
            case CommonStructs::DebuffType::Fire:
                _communication->notifyBinary(data, sizeof(CommonStructs::FireDebuff));
                data_ptr += sizeof(CommonStructs::FireDebuff);
                remaining_data -= sizeof(CommonStructs::FireDebuff);
                break;
            case CommonStructs::DebuffType::Poison:
                _communication->notifyBinary(data, sizeof(CommonStructs::PoisonDebuff));
                data_ptr += sizeof(CommonStructs::PoisonDebuff);
                remaining_data -= sizeof(CommonStructs::PoisonDebuff);
                break;
            case CommonStructs::DebuffType::DeHeal:
                _communication->notifyBinary(data, sizeof(CommonStructs::DeHealDebuff));
                data_ptr += sizeof(CommonStructs::DeHealDebuff);
                remaining_data -= sizeof(CommonStructs::DeHealDebuff);
                break;
            case CommonStructs::DebuffType::Silence:
                _communication->notifyBinary(data, sizeof(CommonStructs::SilenceDebuff));
                data_ptr += sizeof(CommonStructs::SilenceDebuff);
                remaining_data -= sizeof(CommonStructs::SilenceDebuff);
                break;
            case CommonStructs::DebuffType::Exposed:
                _communication->notifyBinary(data, sizeof(CommonStructs::ExposedDebuff));
                data_ptr += sizeof(CommonStructs::ExposedDebuff);
                remaining_data -= sizeof(CommonStructs::ExposedDebuff);
                break;
            case CommonStructs::DebuffType::Shattered:
                _communication->notifyBinary(data, sizeof(CommonStructs::ShartteredDebuff));
                data_ptr += sizeof(CommonStructs::ShartteredDebuff);
                remaining_data -= sizeof(CommonStructs::ShartteredDebuff);
                break;
            }
            break;
        case CommonStructs::MessageType::HealthArmorReport:
            _communication->notifyBinary(data, sizeof(GameStructs::HealthArmorReport));
            data_ptr += sizeof(GameStructs::HealthArmorReport);
            remaining_data -= sizeof(GameStructs::HealthArmorReport);
            break;
        case CommonStructs::MessageType::DamageReport:
            _communication->notifyBinary(data, sizeof(GameStructs::DamageReport));
            data_ptr += sizeof(GameStructs::DamageReport);
            remaining_data -= sizeof(GameStructs::DamageReport);
            break;
        case CommonStructs::MessageType::HealingReport:
            _communication->notifyBinary(data, sizeof(GameStructs::HealingReport));
            data_ptr += sizeof(GameStructs::HealingReport);
            remaining_data -= sizeof(GameStructs::HealingReport);
            break;
        case CommonStructs::MessageType::SSketchReport:
            _communication->notifyBinary(data, sizeof(GameStructs::SSketchReport));
            data_ptr += sizeof(GameStructs::SSketchReport);
            remaining_data -= sizeof(GameStructs::SSketchReport);
            break;
        case CommonStructs::MessageType::IsDeadReport:
            _communication->notifyBinary(data, 1);
            data_ptr += 1;
            remaining_data -= 1;
            break;
        default:
            break;
        }
        processBatchedData(data_ptr, remaining_data);
    }
    else
        return;
}