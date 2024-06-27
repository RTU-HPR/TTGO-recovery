#include "systemConfig.h"

Config::Config(Radio *radio, SPIClass *spi) : _radio(radio), _spi(spi)
{
}

Config::~Config()
{
}

void Config::begin()
{
    EEPROM.begin(EEPROM_SIZE);
    _load();
    _radioConfig.cs = TTGO_LORA_CS;
    _radioConfig.dio0 = TTGO_LORA_DIO0;
    _radioConfig.reset = TTGO_LORA_RST;
    _radioConfig.tx_power = LORA_TX_POWER;
    _radioConfig.spi_bus = _spi;
    _store();
    Serial.println("Config loaded");
}

void Config::_store()
{
    EEPROM.put(EEPROM_LORA_CONFIG_ADDR, _radioConfig);
    EEPROM.commit();
}

void Config::_load()
{
    EEPROM.get(EEPROM_LORA_CONFIG_ADDR, _radioConfig);
}

float Config::frequency()
{
    return _radioConfig.frequency;
}

bool Config::frequency(float frequency)
{
    _radioConfig.frequency = frequency;
    if (_radio->reconfigure(_radioConfig))
    {
        _store();
        return true;
    }
    return false;
}

uint8_t Config::spreadingFactor()
{
    return _radioConfig.spreading;
}

bool Config::spreadingFactor(uint8_t spreading)
{
    _radioConfig.spreading = spreading;
    if (_radio->reconfigure(_radioConfig))
    {
        _store();
        return true;
    }
    return false;
}

float Config::bandwidth()
{
    return _radioConfig.signal_bw;
}

bool Config::bandwidth(float bandwidth)
{
    _radioConfig.signal_bw = bandwidth;
    if (_radio->reconfigure(_radioConfig))
    {
        _store();
        return true;
    }
    return false;
}

uint8_t Config::codingRate()
{
    return _radioConfig.coding_rate;
}

bool Config::codingRate(uint8_t codingRate)
{
    _radioConfig.coding_rate = codingRate;
    if (_radio->reconfigure(_radioConfig))
    {
        _store();
        return true;
    }
    return false;
}

Radio::Config Config::radioConfig()
{
    return _radioConfig;
}