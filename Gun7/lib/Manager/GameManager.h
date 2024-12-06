#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Led.h"
#include "Button.h"
#include "GameStructs.h"
#include "Infrared.h"
#include "PlayerData.h"
#include "Effects.h"
#include "Display.h"
#include "NetworkFacade.h"
#include "UpgradeChip.h"
#include <unordered_map>
#include <esp_system.h>
#include <memory>

class GameManager : public Observer
{
private:
    /* Central variables */
    std::unordered_map<uint8_t, std::tuple<uint8_t, const uint8_t *, const uint8_t *>> _playerMap;

    const char *_enumIndexes[17] = {"Ping", "Pong", "Debuff", "HealthArmorReport", "ConfirmPacket",
                                    "DamagePacket", "HealingPacket", "SSketchPacket", "DamageReport", "HealingReport", "SSketchReport",
                                    "BulletReport", "Pairing", "MappingPlayers", "VestStatPacket", "IsDeadReport", "UpgradeChipReport"};
    const char *_debuffIndexes[6] = {"Fire", "Poison", "DeHeal", "Silence", "Exposed", "Shattered"};

    /* Components Declearation */
    Button _shootButton;
    Button _reloadButton;
    Button _switchButton;
    Infrared _infrared;
    Led _deadLed;

    /* Instance Creation */
    GameStructs::FlowControl _flowControl;
    GameStructs::Mode _currentMode;
    CommonStructs::MessageHeader _header;
    CommonStructs::DebuffHeader _debuffHeader;

    /* Class Instances */
    PlayerData _playerData;
    Effects _effects;
    Communication _communication;
    WServer _server;
    WSocket _socket;
    EspNow _espNow;
    PingPong _ping;
    NetworkFacade _networkFacade;
    Display _display;
    UpgradeChip _upgradeChip;

    /*Template creation, avoiding constant object creation*/
    GameStructs::DamagePacket _damagePacket;           // For decode
    GameStructs::HealingPacket _healingPacket;         // For decode
    GameStructs::SSketchPacket _sSketchPacket;         // For decode
    GameStructs::HealthArmorReport _healthArmorReport; // For decode
    GameStructs::DamageReport _damageReport;           // For decode
    GameStructs::HealingReport _healingReport;         // For decode
    GameStructs::SSketchReport _sSketchReport;         // For decode
    GameStructs::BulletReport _bulletReport;           // For decode
    GameStructs::UpgradeChipReport _upgradeChipReport; // For decode

    GameStructs::ConfirmPacket _confirmPacket; // For response

    CommonStructs::FireDebuff _fireDisplay;            // For display
    CommonStructs::PoisonDebuff _poisonDisplay;        // For display
    CommonStructs::DeHealDebuff _deHealDisplay;        // For display
    CommonStructs::SilenceDebuff _silenceDisplay;      // For display
    CommonStructs::ExposedDebuff _exposedDisplay;      // For display
    CommonStructs::ShartteredDebuff _shatteredDisplay; // For display

    /* Implement Observer update method */
    void updateMesg(const std::string &message) override;
    void updateJson(const JsonDocument &json) override;
    void updateBinary(const uint8_t *data, size_t size) override;

    /* Generic deserialize template */
    template <typename T>
    bool deserialize(T &data, const uint8_t *buffer)
    {
        memcpy(&data, buffer, sizeof(T));
        return true;
    }

    /* Analyze message based on type */
    void analyzeMessage(CommonStructs::MessageType type, const uint8_t *buffer);

public:
    GameManager();
    void init();
    void doNothing(); // For buttons

    /*Self handling functions*/
    void isVestConnected();
    void isPhoneConnected();
    bool reportVestConnected();

    /*Flow control functions*/
    void setBattlePhase(bool state);
    void setValidToPlay();
    void setReadyForBattle(const JsonDocument &json);
    void setStopBattling();
    void setIsDead();
    void setNotDead();
    void setRevival();

    /*Network handling functions*/
    void wServerHandlingClient();
    void espNowHandlingPeer();
    void sendMessageEspNowLoop();
    void sendBinaryToWSocketLoop();

    /* Functions for team registering */
    void addPlayerToMap(uint8_t id, uint8_t teamId, const uint8_t *gunMac, const uint8_t *vestMac);
    void teamRegistering(const JsonDocument &json);
    void selfRegistering();
    void registerPlayersAsPeers();
    void printPlayerMap();
    void printMacAddress(const uint8_t *mac);
    void sendMappingPlayerToVest(uint8_t id, uint8_t teamId, uint8_t *gunMac, uint8_t *vestMac);
    const uint8_t *findMacAddressByIdInMap(uint8_t id);

    /* Functions for analyze, deserialize */
    void analyzeAndDisplayDebuffs(CommonStructs::DebuffHeader debuffHeader, const uint8_t *buffer);

    /* Functions for serialize */
    void sendDamagePacket(const uint8_t *mac);
    void sendHealingPacket(const uint8_t *mac);
    void sendSSketchPacket(const uint8_t *mac);
    void sendBulletReport();
    void sendVestStatPacket(const JsonDocument &json);
    void sendMacSubmission();
    void sendUpgradeChipReport();

    /*Functions for game play*/
    void responseToConfirmPackets(const uint8_t *buffer);

    /* Aux functions */
    void updateDebuffDisplay();
    void updateBulletDisplay();
    void updateInformationDisplay();
    void forceUpdateDebuffDisplay();
    void forceUpdateBulletDisplay();
    void forceUpdateInformationDisplay();

    /*Hardwares control functions*/
    void shot();
    void reload();
    void switchBullet();
    void activateButtons();
    void checkReloadStatus();
};

#endif