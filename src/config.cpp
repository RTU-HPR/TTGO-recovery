#include "main.h"

void storeConfig(wifiLoraConfig *config)
{
    EEPROM.put(EEPROM_LORA_CONFIG_ADDR, *config);
    EEPROM.commit();
}

void loadConfig(wifiLoraConfig *config)
{
    EEPROM.get(EEPROM_LORA_CONFIG_ADDR, *config);
}