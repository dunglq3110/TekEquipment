#include "Display.h"

void Display::setInformationSection(uint16_t health, uint16_t armor, uint16_t damage, uint8_t regenAmount)
{
    statsHousing.health = health;
    statsHousing.armor = armor;
    statsHousing.damage = damage;
    statsHousing.regenAmount = regenAmount;

    drawInformationSection();
}

void Display::setBulletSection(uint8_t normalBullet, uint8_t maxNormalBullet, uint8_t sSketchBullet, uint8_t maxSSketchBullet, bool hasHealing, bool isHealing, bool hasSSketch, bool isSSketch)
{
    statsHousing.normalBullet = normalBullet;
    statsHousing.maxNormalBullet = maxNormalBullet;
    statsHousing.sSketchBullet = sSketchBullet;
    statsHousing.maxSSketchBullet = maxSSketchBullet;
    statsHousing.hasHealing = hasHealing;
    statsHousing.isHealing = isHealing;
    statsHousing.hasSSketch = hasSSketch;
    statsHousing.isSSketch = isSSketch;

    drawBulletSection();
}

void Display::setIsDead(bool isDead)
{
    stateHousing.isDead = isDead;

    if (prevStateHousing.isDead != stateHousing.isDead)
    {
        drawIsDead();
        prevStateHousing.isDead = stateHousing.isDead;
    }
}

void Display::drawInformationSection()
{
    /* Start drawing */
    _infoSprite.startWrite();

    /* Clear the sprite first */
    _infoSprite.fillSprite(TFT_BLACK);
    _infoSprite.setTextSize(1);

    /* Draw the health, armor, damage and regen */
    _infoSprite.setTextColor(TFTU_WHITE);
    _infoSprite.drawBitmap(4, 4, epd_bitmap_Health_Small, 13, 12, TFTU_WHITE);
    _infoSprite.drawString(std::to_string(statsHousing.health).c_str(), 22, 6, 1);

    _infoSprite.setTextColor(TFTU_CYAN);
    _infoSprite.drawBitmap(5, 22, epd_bitmap_Armor_Small, 11, 12, TFTU_CYAN);
    _infoSprite.drawString(std::to_string(statsHousing.armor).c_str(), 22, 23, 1);

    _infoSprite.setTextColor(TFTU_WHITE);
    _infoSprite.drawBitmap(5, 38, epd_bitmap_Damage_Small, 12, 12, TFTU_WHITE);
    _infoSprite.drawString(std::to_string(statsHousing.damage).c_str(), 22, 40, 1);

    _infoSprite.setTextColor(TFTU_CYAN);
    _infoSprite.drawBitmap(4, 55, epd_bitmap_Regen_Small, 13, 12, TFTU_CYAN);
    std::string regenStr = std::to_string(statsHousing.regenAmount) /*+ "|s"*/;
    _infoSprite.drawString(regenStr.c_str(), 22, 57, 1);

    /* Stop drawing and push change */
    _infoSprite.endWrite();
    _infoSprite.pushSprite(2, 2);

    /* Draw the outer frame, debug purposes */
    // drawBorder(1);
}

void Display::drawBulletSection()
{
    /* Start drawing */
    _bulletSprite.startWrite();

    /* Clear the sprite first */
    _bulletSprite.fillSprite(TFTU_BLACK);
    _bulletSprite.setTextSize(1);

    /* Draw the health, armor, damage and regen */
    _bulletSprite.drawBitmap(4, 4, epd_bitmap_Bullet_Normal, 17, 17, !statsHousing.isHealing && !statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE);
    _bulletSprite.drawBitmap(21, 4, epd_bitmap_Bullet_Healing, 17, 17, statsHousing.hasHealing ? statsHousing.isHealing ? TFTU_CYAN : TFTU_WHITE : TFTU_DARKGREY);
    _bulletSprite.drawBitmap(41, 4, epd_bitmap_Bullet_StructuralSketch, 17, 17, statsHousing.hasSSketch ? statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE : TFTU_DARKGREY);

    /* Spacing lines to group normal and healing bullets */
    _bulletSprite.drawLine(4, 24, 34, 24, !statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE);

    /* Spacing line for the structural sketch bullet */
    _bulletSprite.drawLine(40, 24, 59, 24, statsHousing.hasSSketch ? statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE : TFTU_DARKGREY);

    /* Spacing lines between current and max bullet */
    _bulletSprite.drawLine(10, 44, 27, 44, !statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE);
    _bulletSprite.drawLine(40, 44, 58, 44, statsHousing.hasSSketch ? statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE : TFTU_DARKGREY);

    /* Draw the current and max bullet count */
    _bulletSprite.setTextSize(2);
    _bulletSprite.setTextColor(!statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE);
    _bulletSprite.drawString(std::to_string(statsHousing.normalBullet).c_str(), statsHousing.normalBullet > 9 ? 8 : 14, 28, 1);
    _bulletSprite.setTextSize(1);
    _bulletSprite.setTextColor(!statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE);
    _bulletSprite.drawString(std::to_string(statsHousing.maxNormalBullet).c_str(), statsHousing.maxNormalBullet > 9 ? 14 : 17, 47, 1);

    /* Draw the current and max Structural Sketch bullet count */
    _bulletSprite.setTextSize(2);
    _bulletSprite.setTextColor(statsHousing.hasSSketch ? statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE : TFTU_DARKGREY);
    _bulletSprite.drawString(std::to_string(statsHousing.sSketchBullet).c_str(), statsHousing.sSketchBullet > 9 ? 38 : 44, 28, 1);
    _bulletSprite.setTextSize(1);
    _bulletSprite.setTextColor(statsHousing.hasSSketch ? statsHousing.isSSketch ? TFTU_CYAN : TFTU_WHITE : TFTU_DARKGREY);
    _bulletSprite.drawString(std::to_string(statsHousing.maxSSketchBullet).c_str(), statsHousing.maxSSketchBullet > 9 ? 44 : 47, 47, 1);

    /* The line between the name of the current bullet and the rest above */
    _bulletSprite.drawLine(0, 56, 63, 56, TFTU_WHITE);
    _bulletSprite.drawLine(0, 57, 63, 57, TFTU_WHITE);

    /* Draw the bullet type based on current selected bullet */
    _bulletSprite.setTextSize(1);
    _bulletSprite.setTextColor(TFTU_CYAN);
    _bulletSprite.drawString(!statsHousing.isSSketch ? statsHousing.isHealing ? "HEALING" : "NORMAL" : "S.SKETCH", !statsHousing.isSSketch ? statsHousing.isHealing ? 12 : 16 : 8, 61, 1);

    /* Stop drawing and push change */
    _bulletSprite.endWrite();
    _bulletSprite.pushSprite(61, 2);

    /* Draw the outer frame, debug purposes */
    // drawBorder(1);
}

void Display::drawIsDead()
{
    /* Start drawing */
    _connectionSprite.startWrite();

    /* Draw the bitmap and text */
    _connectionSprite.drawBitmap(32, 22, epd_bitmap_player_died, 64, 64, TFTU_WHITE);
    _connectionSprite.drawString("YOU ARE DEAD :<", 20, 93, 1);

    /* Stop drawing and push change */
    _connectionSprite.endWrite();
    _connectionSprite.pushSprite(0, 0);

    /* Draw the outer frame */
    drawBorder(2);
}

void Display::fullDisplayReset()
{
    /* Clear all sprites */
    _infoSprite.fillSprite(TFT_BLACK);
    _bulletSprite.fillSprite(TFT_BLACK);
    _debuffSprite.fillSprite(TFT_BLACK);
    _connectionSprite.fillSprite(TFT_BLACK);

    /* Push the changes */
    _infoSprite.pushSprite(2, 2);
    _bulletSprite.pushSprite(61, 2);
    _debuffSprite.pushSprite(2, 75);
    _connectionSprite.pushSprite(0, 0);

    /* Reset all structs */
    debuffHousing = {};
    statsHousing = {};
    stateHousing = {};
    prevStateHousing = {};

    /* Draw the outer frame */
    drawBorder(0);
}