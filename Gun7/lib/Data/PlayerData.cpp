#include "PlayerData.h"
#include <Arduino.h>

PlayerData::PlayerData()
{
}

void PlayerData::setStats(const JsonDocument &json)
{
    stats.damage = json["damage_value"];
    stats.heal = json["healing_value"];
    stats.maxBullet = json["bullet_max"];
    stats.maxSSketchBullet = json["ssketch_bullet_max"];
    stats.curBullet = stats.maxBullet;
    stats.curSSketchBullet = stats.maxSSketchBullet;
    stats.bulletTime = json["bullet_reload_time"];
    stats.sSketchBulletTime = json["ssketch_bullet_reload_time"];
    stats.lifeStealValue = json["life_steal_value"];
    stats.playerHasHeal = stats.heal > 0 ? true : false;
    stats.playerHasSSketch = stats.maxSSketchBullet > 0 ? true : false;
    stats.playerHasTrueDamage = json["has_true_damage"];
}

void PlayerData::setHealthArmor(const JsonDocument &json)
{
    stats.health = json["health_max"];
    stats.armor = json["armor_max"];
}

void PlayerData::resetStats()
{
    stats = {};
    state = {};
}