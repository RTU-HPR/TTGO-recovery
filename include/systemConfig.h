#pragma once

// ======================== Library Includes ========================
#include <Arduino.h>
#include <EEPROM.h>
#include <MUFFINS_Radio.h>
#include "config.h"


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
