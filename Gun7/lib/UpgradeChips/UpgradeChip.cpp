#include "UpgradeChip.h"

UpgradeChip::UpgradeChip()
{
}

void UpgradeChip::init()
{
    Wire.begin(21, 22);
}

uint8_t UpgradeChip::readMostCommon()
{
    uint8_t result = 0; // Default value in case of failure
    uint8_t values[5] = {0};
    int retries = 0;

    while (retries < MAX_RETRIES)
    {
        // Read data 5 times
        for (int i = 0; i < 5; i++)
        {
            // Attempt to communicate with the EEPROM
            if (!isEEPROMConnected())
            {
                Serial.println("EEPROM not connected or not responding.");
                return 0xFF; // Indicate error if the device is not responding
            }

            Wire.beginTransmission(EEPROM_ADDRESS);
            Wire.write(0);                   // Send the address to read from
            if (Wire.endTransmission() != 0) // If it fails to communicate, return error
            {
                Serial.println("Failed to communicate with EEPROM.");
                return 0xFF; // Return an error code
            }

            Wire.requestFrom(EEPROM_ADDRESS, 1);
            if (Wire.available())
            {
                values[i] = Wire.read();
                Serial.print("Read from EEPROM: 0x");
                Serial.println(values[i], HEX);
            }
            else
            {
                Serial.println("Failed to read from EEPROM");
                values[i] = 0xFF; // Placeholder for failed reads
            }
        }

        // Check if a valid majority exists
        if (isValidMajority(values, result))
        {
            return result; // Return the majority value if found
        }

        retries++;
        Serial.print("Retry attempt ");
        Serial.println(retries);
    }

    // If all retries fail, indicate chip fault by returning a special value (0xFF or something else)
    Serial.println("Chip is faulty.");
    return 0xFF; // Return a special value to indicate failure (can be modified)
}

bool UpgradeChip::isValidMajority(const uint8_t values[], uint8_t &result)
{
    int count[256] = {0};

    // Count occurrences of each value
    for (int i = 0; i < 5; i++)
    {
        count[values[i]]++;
    }

    // Find the value with maximum count
    for (int i = 0; i < 256; i++)
    {
        if (count[i] >= 4) // Check if it meets the majority condition
        {
            result = i;
            return true;
        }
    }

    return false; // No valid majority found
}

bool UpgradeChip::isEEPROMConnected()
{
    // This function checks if the EEPROM device is responding on the I2C bus
    Wire.beginTransmission(EEPROM_ADDRESS);
    if (Wire.endTransmission() == 0) // If the transmission ends successfully, the device is present
    {
        return true;
    }
    else
    {
        return false; // If transmission fails, the device is not responding
    }
}
