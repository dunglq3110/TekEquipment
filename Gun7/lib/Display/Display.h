#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include <SPI.h>
#include "DisplayStructs.h"
#include "Bitmap.h"

class Display
{
private:
    TFT_eSPI _tft = TFT_eSPI();
    TFT_eSprite _bulletSprite = TFT_eSprite(&_tft);
    TFT_eSprite _infoSprite = TFT_eSprite(&_tft);
    TFT_eSprite _debuffSprite = TFT_eSprite(&_tft);
    TFT_eSprite _connectionSprite = TFT_eSprite(&_tft);
    TFT_eSprite _findingSprite = TFT_eSprite(&_tft);

public:
    Display();
    DisplayStructs::DebuffHousing debuffHousing;
    DisplayStructs::StatsHousing statsHousing;
    DisplayStructs::StateHousing stateHousing;
    DisplayStructs::StateHousing prevStateHousing;

    void init();

    // Common
    // 0: Connections, 1: Battle
    void drawBorder(uint8_t type);
    void drawBorderOuter();
    void drawBorderConnections();
    void drawBorderBattle();
    void drawWiFiFinding();

    // Pregame setters
    void setWiFiStatus(bool isConnected, bool isPlayable, std::string macAddress);
    void setConnectionsState(bool isVest, bool isPhone, bool isReady, std::string ip, std::string channel);

    // Ingame setters
    void setInformationSection(uint16_t health, uint16_t armor,
                               uint16_t damage, uint8_t regenAmount);
    void setBulletSection(uint8_t normalBullet, uint8_t maxNormalBullet,
                          uint8_t sSketchBullet, uint8_t maxSSketchBullet,
                          bool hasHealing, bool isHealing,
                          bool hasSSketch, bool isSSketch);
    void setDebuffSection(uint8_t fireLvl, uint8_t fireDuration,
                          uint8_t poisonLvl, uint8_t poisonDuration,
                          uint8_t deHealLvl, uint8_t deHealDuration,
                          uint8_t exposedLvl, uint8_t exposedDuration,
                          uint8_t shatteredLvl, uint8_t shatteredDuration,
                          uint8_t weakenedLvl, uint8_t weakenedDuration,
                          uint8_t bleedingLvl, uint8_t bleedingDuration,
                          uint8_t engulfedLvl, uint8_t engulfedDuration);
    void setIsDead(bool isDead);

    // Pregame
    void drawWiFiStatus();
    void drawConnectionsState();

    // Ingame
    void drawInformationSection();
    void drawBulletSection();
    void drawDebuffSection();
    void drawIsDead();

    // Endgame
    void fullDisplayReset();
};

#endif