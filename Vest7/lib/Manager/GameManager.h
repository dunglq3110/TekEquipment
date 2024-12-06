#ifndef GAMEFACADE_H
#define GAMEFACADE_H

#include "DamageCalcualtion.h"
#include <unordered_map>
#include "IRreceiver.h"
#include "EspNow.h"
#include "Effects.h"
#include "PingPong.h"

class GameManager : public Observer
{
private:
    /*Central variables*/
    std::unordered_map<uint8_t, std::tuple<uint8_t, const uint8_t *, const uint8_t *>> _playerMap;

    const char *_enumIndexes[16] = {"Ping", "Pong", "Debuff", "HealthArmorReport", "ConfirmPacket",
                                    "DamagePacket", "HealingPacket", "SSketchPacket", "DamageReport", "HealingReport", "SSketchReport",
                                    "BulletReport", "Pairing", "MappingPlayers", "VestStatPacket", "IsDeadReport"};
    const char *_debuffIndexes[6] = {"Fire", "Poison", "DeHeal", "Silence", "Exposed", "Shattered"};

    /*Instance Creation*/
    CommonStructs::MessageHeader _header;
    CommonStructs::DebuffHeader _debuffHeader;

    /* Class Instances */
    DamageCalculation _damageCalculation;
    PlayerData _playerData;
    Communication _communication;
    PingPong _pong;
    EspNow _espNow;
    IRreceiver _irrecv;
    Effects _effect;

    /*Template creation, avoiding constant object creation*/
    GameStructs::ConfirmPacket _confirmPacket;
    GameStructs::DamagePacket _damagePacket;
    GameStructs::HealingPacket _healingPacket;
    GameStructs::SSketchPacket _sSketchPacket;
    GameStructs::HealthArmorReport _healthArmorReport;
    GameStructs::DamageReport _damageReport;
    GameStructs::HealingReport _healingReport;
    GameStructs::SSketchReport _sSketchReport;

    CommonStructs::FireDebuff _fire;
    CommonStructs::PoisonDebuff _poison;
    CommonStructs::DeHealDebuff _deHeal;
    CommonStructs::SilenceDebuff _silence;
    CommonStructs::ExposedDebuff _exposed;
    CommonStructs::ShartteredDebuff _shattered;

    /*Implement Observer update method*/
    void updateInt(uint64_t uint_64) override;
    void updateJson(const JsonDocument &json) override;
    void updateBinary(const uint8_t *data, size_t size, uint8_t id) override;

    /* Generic deserialize template */
    template <typename T>
    bool deserialize(T &data, const uint8_t *buffer)
    {
        memcpy(&data, buffer, sizeof(T));
        return true;
    }

public:
    GameManager();
    void init();

    /*Functions for TeamRegistering and self registering*/
    void addPlayerToMap(uint8_t id, uint8_t teamId, const uint8_t *GunMacAddr, const uint8_t *VestMacAddr);
    void playerRegistering(uint8_t id, uint8_t teamId, const uint8_t *GunMacAddr, const uint8_t *VestMacAddr);
    void registerPlayersAsPeers();
    void printPlayerMap();
    void printMacAddress(const uint8_t *macAddr);

    /* Functions for deserialize */
    void decodePlayerMap(const uint8_t *buffer);
    void decodeVestStatPacket(const uint8_t *buffer);
    void decodeDamagePacket(const uint8_t *buffer);
    void decodeHealingPacket(const uint8_t *buffer);
    void decodeSSketchPacket(const uint8_t *buffer);

    /* Functions for serialize */
    void sendConfirmPacket(const uint8_t *playerGunMacAddr);
    void sendHealthArmorReport();
    void sendDamageReport(uint8_t taggerId, uint16_t damage);
    void sendHealingReport(uint8_t taggerId, uint8_t heal);
    void sendSSketchReport(uint8_t taggerId, uint16_t damage);
    void sendIsDeadReport();

    /*Self handling functions*/
    void isGunConnected();
    void isPlayerDead();

    /*Functions for executing the game itself like response IR packet,...*/
    const uint8_t *findMacAddressByIdInMap(uint64_t id);
    void runEffects();

    /*Network handling functions*/
    void runningIRRecv();
    void espnowHandlingPeer();
    void sendMessageEspNowLoop();

    /*Helper Functions*/
    bool convertMessageToHexAndSplit(const std::string &message, uint8_t *mac, uint8_t &channel);
};

#endif