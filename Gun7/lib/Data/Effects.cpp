#include "Effects.h"

Effects::Effects()
{
}

void Effects::setDebuffs(const JsonDocument &json)
{
    // Set debuffs rates
    rates.fire = json["fire_rate"];
    rates.poison = json["poison_rate"];
    rates.silence = json["silence_rate"];

    fireOrg.level = json["fire_level"];
    fireOrg.duration = json["fire_duration"];
    fireOrg.value = json["fire_value"];
    fireOrg.hasTrueDamage = json["fire_has_true_damage"];

    poisonOrg.level = json["poison_level"];
    poisonOrg.duration = json["poison_duration"];
    poisonOrg.value = json["poison_value"];

    deHealOrg.level = json["deheal_level"];
    deHealOrg.duration = json["deheal_duration"];
    deHealOrg.healReduction = json["deheal_heal_reduction"];
    deHealOrg.blockRegen = json["deheal_block_regen"];

    silenceOrg.level = json["silence_level"];
    silenceOrg.duration = json["silence_duration"];
    silenceOrg.armorDecrease = json["silence_armor_decrease"];

    exposedOrg.level = json["exposed_level"];
    exposedOrg.duration = json["exposed_duration"];
    exposedOrg.armorDecrease = json["exposed_armor_decrease"];
    exposedOrg.damageVulnerability = json["exposed_damage_vulnerability"];
}

void Effects::calculateDebuff()
{
    bool Fire_Proc = random(101) < rates.fire;
    bool Poison_Proc = random(101) < rates.poison;
    bool DeHeal_Proc = Poison_Proc;
    bool Silence_Proc = random(101) < rates.silence;

    if (Fire_Proc)
    {
        fireMod = fireOrg;
    }
    else
    {
        fireMod = {};
    }

    if (Poison_Proc)
    {
        poisonMod = poisonOrg;
    }
    else
    {
        poisonMod = {};
    }

    if (DeHeal_Proc)
    {
        deHealMod = deHealOrg;
    }
    else
    {
        deHealMod = {};
    }

    if (Silence_Proc)
    {
        silenceMod = silenceOrg;
    }
    else
    {
        silenceMod = {};
    }
}

void Effects::resetEffects()
{
    fireOrg = {};
    poisonOrg = {};
    deHealOrg = {};
    silenceOrg = {};
    exposedOrg = {};
    shatteredOrg = {};

    fireMod = {};
    poisonMod = {};
    deHealMod = {};
    silenceMod = {};
}