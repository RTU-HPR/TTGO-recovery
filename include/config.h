#pragma once
#include "main.h"

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

    float frequency();
    bool frequency(float frequency);

    uint8_t spreadingFactor();
    bool spreadingFactor(uint8_t spreadingFactor);

    uint16_t bandwidth();
    bool bandwidth(uint16_t bandwidth);

    uint8_t codingRate();
    bool codingRate(uint8_t codingRate);

    Radio::Config radioConfig();
};
