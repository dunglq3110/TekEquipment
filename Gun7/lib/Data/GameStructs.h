#ifndef GAMESTRUCTS_H
#define GAMESTRUCTS_H
#include "CommonStructs.h"

class GameStructs
{
public:
    struct FlowControl
    {
        bool isPreGamePhase = false;
        bool isBattlePhase = false;
        bool isValidToPlay = false;
        bool isDrawBattleFirstTime = false;
    };

    struct PingPacket
    {
        CommonStructs::MessageHeader header;
    };

    struct DeadPacket
    {
        CommonStructs::MessageHeader header;
    };

    struct ConfirmPacket
    {
        CommonStructs::MessageHeader header;
        uint8_t id;
        uint8_t teamId;
    };

    struct DamagePacket
    {
        CommonStructs::MessageHeader header;
        uint8_t id;
        uint8_t teamId;
        uint16_t damage;
        uint8_t lifeStealValue;
        CommonStructs::FireDebuff fire;
        CommonStructs::PoisonDebuff poison;
        CommonStructs::DeHealDebuff deHeal;
        CommonStructs::SilenceDebuff silence;
    };

    struct HealingPacket
    {
        CommonStructs::MessageHeader header;
        uint8_t id;
        uint8_t teamId;
        uint8_t heal;
    };

    struct SSketchPacket
    {
        CommonStructs::MessageHeader header;
        uint8_t id;
        uint8_t teamId;
        uint16_t damage;
        CommonStructs::ExposedDebuff exposed;
    };

    struct __attribute__((packed)) MappingPlayers
    {
        CommonStructs::MessageHeader header;
        uint8_t gunMacAddr[6];
        uint8_t vestMacAddr[6];
        uint8_t id;
        uint8_t teamId;
    };

    struct HealthArmorReport
    {
        CommonStructs::MessageHeader header;
        uint8_t id;
        uint16_t health;
        uint16_t armor;
    };

    struct DamageReport
    {
        CommonStructs::MessageHeader header;
        uint8_t taggerId;
        uint8_t victimId;
        uint16_t damage;
    };

    struct HealingReport
    {
        CommonStructs::MessageHeader header;
        uint8_t healerId;
        uint8_t healedId;
        uint8_t heal;
    };

    struct SSketchReport
    {
        CommonStructs::MessageHeader header;
        uint8_t taggerId;
        uint8_t victimId;
        uint16_t damage;
    };

    struct BulletReport
    {
        CommonStructs::MessageHeader header;
        uint8_t id;
        uint8_t normalBullets;
        uint8_t ssketchBullets;
    };

    struct UpgradeChipReport
    {
        CommonStructs::MessageHeader header;
        uint8_t id;
        uint8_t chipId;
    };

    // Bullet Modes
    enum Mode
    {
        NORMAL,
        HEALING,
        SSKETCH
    };

    // For sending stats to vest for each
    // round after receiving from phone websocket
    struct VestStatPacket
    {
        CommonStructs::MessageHeader header;
        uint16_t health = 0;
        uint16_t armor = 0;
        uint16_t maxHealth = 0;
        uint16_t maxArmor = 0;
        uint8_t armorIncrease = 0;
        uint8_t armorDecrease = 0;
        uint8_t extraDamageRecv = 0;
        uint8_t baseDamageVulnerability = 0;
        uint8_t baseDamageResistance = 0;
        uint8_t bonusDamageVulnerability = 0;
        uint8_t bonusDamageResistance = 0;
    };
};

#endif