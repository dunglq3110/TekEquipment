#ifndef UPGRADECHIP_H
#define UPGRADECHIP_H

#include <Wire.h>
#include <Arduino.h>

#define EEPROM_ADDRESS 0x50
#define MAX_RETRIES 4

class UpgradeChip
{
private:
    bool isValidMajority(const uint8_t values[], uint8_t &result);
    bool isEEPROMConnected();

public:
    UpgradeChip();
    void init();
    uint8_t readMostCommon();
};

#endif