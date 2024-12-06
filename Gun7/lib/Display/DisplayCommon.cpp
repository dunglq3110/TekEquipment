#include "Display.h"

Display::Display()
{
}

void Display::init()
{
    _tft.init();
    _tft.setTextWrap(true);
    _tft.fillScreen(TFT_BLACK);
    _tft.setTextFont(1);

    _infoSprite.createSprite(57, 71);
    _bulletSprite.createSprite(64, 71);
    _debuffSprite.createSprite(123, 51);
    _connectionSprite.createSprite(126, 126);
    _findingSprite.createSprite(126, 126);
}

void Display::drawBorder(uint8_t type)
{
    drawBorderOuter();

    if (type == 0)
    {
        drawBorderConnections();
    }
    else if (type == 1)
    {
        drawBorderBattle();
    }
}

void Display::drawBorderOuter()
{
    _tft.drawRect(0, 0, 127, 128, TFTU_WHITE);
    _tft.drawRect(1, 1, 125, 126, TFTU_WHITE);
}

void Display::drawBorderConnections()
{
    /* Draw the vertical line */
    _tft.drawLine(63, 0, 63, 109, TFTU_WHITE);
    _tft.drawLine(64, 0, 64, 109, TFTU_WHITE);

    /* Draw the horizontal line */
    _tft.drawLine(63, 49, 126, 49, TFTU_WHITE);
    _tft.drawLine(63, 50, 126, 50, TFTU_WHITE);

    /* Draw the horizontal line that enclosed the channel */
    _tft.drawLine(0, 91, 63, 91, TFTU_WHITE);
    _tft.drawLine(0, 92, 63, 92, TFTU_WHITE);

    /* Draw the border that enclosed the IP */
    _tft.drawRect(0, 108, 127, 19, TFTU_WHITE);
    _tft.drawRect(0, 109, 127, 19, TFTU_WHITE);
}

void Display::drawBorderBattle()
{
    /* Draw the border that enclosed the health, armor and damage */
    _tft.drawRect(0, 0, 60, 74, TFTU_WHITE);
    _tft.drawRect(0, 0, 61, 75, TFTU_WHITE);

    /* Draw the border that enclosed the bullet type's name section also the bullet section itself*/
    _tft.drawRect(60, 59, 64, 2, TFTU_WHITE);
    _tft.drawRect(60, 73, 65, 2, TFTU_WHITE);
}