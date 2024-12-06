#include "Display.h"

void Display::setDebuffSection(uint8_t fireLvl, uint8_t fireDuration,
                               uint8_t poisonLvl, uint8_t poisonDuration,
                               uint8_t deHealLvl, uint8_t deHealDuration,
                               uint8_t exposedLvl, uint8_t exposedDuration,
                               uint8_t shatteredLvl, uint8_t shatteredDuration,
                               uint8_t weakenedLvl, uint8_t weakenedDuration,
                               uint8_t bleedingLvl, uint8_t bleedingDuration,
                               uint8_t engulfedLvl, uint8_t engulfedDuration)
{
    debuffHousing.fireLvl = fireLvl;
    debuffHousing.fireDuration = fireDuration;
    debuffHousing.poisonLvl = poisonLvl;
    debuffHousing.poisonDuration = poisonDuration;
    debuffHousing.deHealLvl = deHealLvl;
    debuffHousing.deHealDuration = deHealDuration;
    debuffHousing.exposedLvl = exposedLvl;
    debuffHousing.exposedDuration = exposedDuration;
    debuffHousing.shatteredLvl = shatteredLvl;
    debuffHousing.shatteredDuration = shatteredDuration;
    debuffHousing.weakenedLvl = weakenedLvl;
    debuffHousing.weakenedDuration = weakenedDuration;
    debuffHousing.bleedingLvl = bleedingLvl;
    debuffHousing.bleedingDuration = bleedingDuration;
    debuffHousing.engulfedLvl = engulfedLvl;
    debuffHousing.engulfedDuration = engulfedDuration;

    drawDebuffSection();
}

void Display::drawDebuffSection()
{
    /* Start drawing */
    _debuffSprite.startWrite();

    /* Clear the sprite first */
    _debuffSprite.fillSprite(TFT_BLACK);
    _debuffSprite.setTextSize(1);

    /* Draw the fire debuff */
    _debuffSprite.setTextColor(TFTU_RED);
    _debuffSprite.drawBitmap(3, 5, debuffHousing.fireLvl == 1 ? epd_bitmap_Fire_1 : epd_bitmap_Fire_2, 13, 17, debuffHousing.fireDuration > 0 ? TFTU_RED : TFTU_BLACK);
    _debuffSprite.setTextColor(debuffHousing.fireDuration > 0 ? TFTU_RED : TFTU_BLACK);
    _debuffSprite.drawString(std::to_string(debuffHousing.fireDuration).c_str(), 17, 15, 1);

    /* Draw the poison debuff */
    _debuffSprite.setTextColor(TFTU_DARKGREEN);
    _debuffSprite.drawBitmap(32, 5, debuffHousing.poisonLvl == 1 ? epd_bitmap_Poison_1 : epd_bitmap_Poison_2, 13, 17, debuffHousing.poisonDuration > 0 ? TFTU_DARKGREEN : TFTU_BLACK);
    _debuffSprite.setTextColor(debuffHousing.poisonDuration > 0 ? TFTU_DARKGREEN : TFTU_BLACK);
    _debuffSprite.drawString(std::to_string(debuffHousing.poisonDuration).c_str(), 48, 15, 1);

    /* Draw the exposed debuff */
    _debuffSprite.setTextColor(TFTU_ORANGE);
    _debuffSprite.drawBitmap(63, 5, debuffHousing.exposedLvl == 1 ? epd_bitmap_Exposed_1 : epd_bitmap_Exposed_2, 13, 17, debuffHousing.exposedDuration > 0 ? TFTU_ORANGE : TFTU_BLACK);
    _debuffSprite.setTextColor(debuffHousing.exposedDuration > 0 ? TFTU_ORANGE : TFTU_BLACK);
    _debuffSprite.drawString(std::to_string(debuffHousing.exposedDuration).c_str(), 79, 15, 1);

    /* Draw the weakened debuff */
    _debuffSprite.setTextColor(TFTU_SILVER);
    _debuffSprite.drawBitmap(94, 5, debuffHousing.weakenedLvl == 1 ? epd_bitmap_Weakened_1 : epd_bitmap_Weakened_2, 13, 17, debuffHousing.weakenedDuration > 0 ? TFTU_SILVER : TFTU_BLACK);
    _debuffSprite.setTextColor(debuffHousing.weakenedDuration > 0 ? TFTU_SILVER : TFTU_BLACK);
    _debuffSprite.drawString(std::to_string(debuffHousing.weakenedDuration).c_str(), 110, 15, 1);

    /* Draw the bleeding debuff */
    _debuffSprite.setTextColor(TFTU_MAROON);
    _debuffSprite.drawBitmap(3, 29, debuffHousing.bleedingLvl == 1 ? epd_bitmap_Bleeding_1 : epd_bitmap_Bleeding_2, 13, 17, debuffHousing.bleedingDuration > 0 ? TFTU_MAROON : TFTU_BLACK);
    _debuffSprite.setTextColor(debuffHousing.bleedingDuration > 0 ? TFTU_MAROON : TFTU_BLACK);
    _debuffSprite.drawString(std::to_string(debuffHousing.bleedingDuration).c_str(), 17, 39, 1);

    /* Draw the deheal debuff */
    _debuffSprite.setTextColor(TFTU_OLIVE);
    _debuffSprite.drawBitmap(32, 29, debuffHousing.deHealLvl == 1 ? epd_bitmap_De_Heal_1 : epd_bitmap_De_Heal_2, 13, 17, debuffHousing.deHealDuration > 0 ? TFTU_OLIVE : TFTU_BLACK);
    _debuffSprite.setTextColor(debuffHousing.deHealDuration > 0 ? TFTU_OLIVE : TFTU_BLACK);
    _debuffSprite.drawString(std::to_string(debuffHousing.deHealDuration).c_str(), 48, 39, 1);

    /* Draw the shattered debuff */
    _debuffSprite.setTextColor(TFTU_CYAN);
    _debuffSprite.drawBitmap(63, 29, debuffHousing.shatteredLvl == 1 ? epd_bitmap_Shattered_1 : epd_bitmap_Shattered_2, 13, 17, debuffHousing.shatteredDuration > 0 ? TFTU_CYAN : TFTU_BLACK);
    _debuffSprite.setTextColor(debuffHousing.shatteredDuration > 0 ? TFTU_CYAN : TFTU_BLACK);
    _debuffSprite.drawString(std::to_string(debuffHousing.shatteredDuration).c_str(), 79, 39, 1);

    /* Draw the engulfed debuff */
    _debuffSprite.setTextColor(TFTU_MAGENTA);
    _debuffSprite.drawBitmap(94, 29, debuffHousing.engulfedLvl == 1 ? epd_bitmap_Endless_Void_1 : epd_bitmap_Endless_Void_2, 13, 17, debuffHousing.engulfedDuration > 0 ? TFTU_MAGENTA : TFTU_BLACK);
    _debuffSprite.setTextColor(debuffHousing.engulfedDuration > 0 ? TFTU_MAGENTA : TFTU_BLACK);
    _debuffSprite.drawString(std::to_string(debuffHousing.engulfedDuration).c_str(), 110, 39, 1);

    /* Stop drawing and push change */
    _debuffSprite.endWrite();
    _debuffSprite.pushSprite(2, 75);

    /* Draw the outer frame, debug purposes */
    // drawBorder(1);
}