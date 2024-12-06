#include "Display.h"

void Display::setConnectionsState(bool isVest, bool isPhone, bool isReady, std::string ip, std::string channel)
{
    stateHousing.isVest = isVest;
    stateHousing.isPhone = isPhone;
    stateHousing.isReady = isReady;
    stateHousing.ip = ip;
    stateHousing.channel = channel;

    if (stateHousing == prevStateHousing)
    {
        return;
    }

    drawConnectionsState();
    prevStateHousing = stateHousing;
}

void Display::setWiFiStatus(bool isConnected, bool isPlayable, std::string macAddress)
{
    stateHousing.isWiFiConnected = isConnected;
    stateHousing.isWiFiPlayable = isPlayable;
    stateHousing.macAddress = macAddress;

    if (stateHousing == prevStateHousing)
    {
        return;
    }

    drawWiFiStatus();
    prevStateHousing = stateHousing;
}

void Display::drawConnectionsState()
{
    /* Start drawing */
    _connectionSprite.startWrite();

    /* Clear the sprite first */
    _connectionSprite.fillSprite(TFT_BLACK);
    _connectionSprite.setTextSize(1);

    /* Clear the connection section */
    _connectionSprite.fillRect(9, 28, 48, 48, TFTU_BLACK);
    _connectionSprite.fillRect(71, 55, 48, 48, TFTU_BLACK);
    _connectionSprite.fillRect(71, 5, 48, 40, TFTU_BLACK);

    /* Redraw all elements (with updated values if changed) */
    _connectionSprite.drawBitmap(9, 28, stateHousing.isVest ? epd_bitmap_vest_yes_48 : epd_bitmap_vest_no_48, 48, 48, TFTU_WHITE);
    _connectionSprite.drawBitmap(71, 55, stateHousing.isPhone ? epd_bitmap_verified_phone_48 : epd_bitmap_declined_phone_48, 48, 48, TFTU_WHITE);
    _connectionSprite.drawBitmap(71, 5, stateHousing.isReady ? epd_bitmap_verified_laptop_48 : epd_bitmap_declined_laptop_48, 48, 48, TFTU_WHITE);

    /* Draw the IP and channel */
    _connectionSprite.drawString("Chan: ", 6, 97, 1);
    _connectionSprite.drawString(stateHousing.channel.c_str(), 37, 97, 1);

    _connectionSprite.drawString("IP: ", 6, 114, 1);
    _connectionSprite.drawString(stateHousing.ip.c_str(), 26, 114, 1);

    /* Stop drawing, push change and set flag to false */
    _connectionSprite.endWrite();
    _connectionSprite.pushSprite(0, 0);

    /* Draw the battle frame */
    drawBorder(0);
}

void Display::drawWiFiStatus()
{
    /* Start drawing */
    _connectionSprite.fillSprite(TFT_BLACK);
    _connectionSprite.startWrite();

    /* Clear the sprite first */
    _connectionSprite.fillSprite(TFT_BLACK);
    _connectionSprite.setTextSize(1);

    if (stateHousing.isWiFiConnected && !stateHousing.isWiFiPlayable)
    {
        /* Draw the bitmap and text */
        _connectionSprite.drawBitmap(32, 16, epd_bitmap_wifi_signal, 64, 64, TFTU_WHITE);
        _connectionSprite.drawString("Connected!", 35, 75, 1);
        _connectionSprite.drawString("Yet unplayable WiFi", 7, 85, 1);
        _connectionSprite.drawString("Either change WiFi", 10, 95, 1);
        _connectionSprite.drawString("or turn on a VPN", 15, 105, 1);
    }
    else if (!stateHousing.isWiFiConnected)
    {
        /* Draw the bitmap and text */
        _connectionSprite.drawBitmap(32, 6, epd_bitmap_no_wifi, 64, 64, TFTU_WHITE);
        _connectionSprite.drawString("No Connection!", 24, 65, 1);
        _connectionSprite.drawString("Hold tops buttons", 13, 75, 1);
        _connectionSprite.drawString("to forget this WiFi", 7, 85, 1);
        _connectionSprite.drawString("Device's WiFi name: ", 8, 105, 1);
        _connectionSprite.drawString(stateHousing.macAddress.c_str(), 13, 115, 1);
    }

    /* Stop drawing and push change */
    _connectionSprite.endWrite();
    _connectionSprite.pushSprite(0, 0);

    /* Draw the battle frame */
    drawBorder(2);
}

void Display::drawWiFiFinding()
{
    /* Start drawing */
    _findingSprite.startWrite();

    /* Clear the sprite first */
    _findingSprite.fillSprite(TFT_BLACK);
    _findingSprite.setTextSize(1);

    /* Draw the bitmap and text */
    _findingSprite.drawBitmap(32, 22, epd_bitmap_wifi_searching, 64, 64, TFTU_WHITE);
    _findingSprite.drawString("Finding WiFi", 30, 81, 1);

    /* Stop drawing and push change */
    _findingSprite.endWrite();
    _findingSprite.pushSprite(0, 0);

    /* Draw the outer frame */
    drawBorder(2);
}