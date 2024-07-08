#pragma once

// ======================== Library Includes ========================
#include <Arduino.h>
#include <EEPROM.h>
#include <MUFFINS_Radio.h>
#include "config.h"

#define EEPROM_SIZE 512           ///< EEPROM size in bytes
#define EEPROM_LORA_CONFIG_ADDR 0 ///< EEPROM address for LoRa configuration

class Config
{
private:
    Radio::Config _radioConfig;
    Radio *_radio;
    SPIClass *_spi;

    void _store();
    void _load();
    void _setDefaults();

public:
    Config(Radio *radio, SPIClass *spi);
    ~Config();

    void begin();

    float frequency();
    bool frequency(float frequency);

    uint8_t spreadingFactor();
    bool spreadingFactor(uint8_t spreadingFactor);

    float bandwidth();
    bool bandwidth(float bandwidth);

    uint8_t codingRate();
    bool codingRate(uint8_t codingRate);

    Radio::Config radioConfig();
};
