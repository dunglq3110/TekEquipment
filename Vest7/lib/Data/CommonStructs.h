#ifndef COMMONSTRUCTS_H
#define COMMONSTRUCTS_H

#include <stdint.h>

class CommonStructs
{
public:
    enum class MessageType : uint8_t
    {
        // For Ping Pong, keeping gun and vest alive, most frequently used
        Ping = 0,
        Pong = 1,

        // For debuffs, health and armor report to the websocket, second most frequently used
        Debuff = 2,
        HealthArmorReport = 3,

        // For damage, healing, sSketch, confirm packet, third frequently used
        ConfirmPacket = 4,
        DamagePacket = 5,
        HealingPacket = 6,
        SSketchPacket = 7,

        // For damage, healing, sSketch, report, fourth frequently used
        DamageReport = 8,
        HealingReport = 9,
        SSketchReport = 10,
        BulletReport = 11,

        // Additional message types, for pairing, match init, least frequently used
        Pairing = 12,
        MappingPlayers = 13,
        VestStatPacket = 14,
        IsDeadReport = 15
    };

    enum class DebuffType : uint8_t
    {
        Fire = 0,
        Poison = 1,
        DeHeal = 2,
        Silence = 3,
        Exposed = 4,
        Shattered = 5
    };

    struct MessageHeader
    {
        MessageType type;
    };

    struct DebuffHeader
    {
        MessageHeader header;
        DebuffType debuffType;
    };

    struct FireDebuff
    {
        DebuffHeader header;
        uint8_t ownerId;
        uint8_t level = 0;
        uint8_t duration = 0;
        uint8_t value = 0;
        bool hasTrueDamage = false;
    };

    struct PoisonDebuff
    {
        DebuffHeader header;
        uint8_t ownerId;
        uint8_t level = 0;
        uint8_t duration = 0;
        uint8_t value = 0;
    };

    struct DeHealDebuff
    {
        DebuffHeader header;
        uint8_t ownerId;
        uint8_t level = 0;
        uint8_t duration = 0;
        uint8_t healReduction = 0;
        bool blockRegen = false;
    };

    struct SilenceDebuff
    {
        DebuffHeader header;
        uint8_t ownerId;
        uint8_t level = 0;
        uint8_t duration = 0;
        uint8_t armorDecrease = 0;
    };

    struct ExposedDebuff
    {
        DebuffHeader header;
        uint8_t ownerId;
        uint8_t level = 0;
        uint8_t duration = 0;
        uint8_t armorDecrease = 0;
        uint8_t damageVulnerability = 0;
    };

    struct ShartteredDebuff
    {
        DebuffHeader header;
        uint8_t ownerId;
        uint8_t level = 0;
        uint8_t duration = 0;
        uint8_t bonusArmor = 0;
        uint8_t extraDamageRecv = 0;
    };

    struct Regeneration
    {
        uint8_t value = 0;
        uint16_t interval;
        unsigned long long lastRegen;
    };
};

#endif