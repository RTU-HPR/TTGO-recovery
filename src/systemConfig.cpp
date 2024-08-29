#include "systemConfig.h"

Config::Config(Radio *radio, MqttClient *mqtt, SPIClass *spi) : _radio(radio), _mqtt(mqtt), _spi(spi)
{
}

Config::~Config()
{
}

void Config::begin()
{
    EEPROM.begin(EEPROM_SIZE);
    if (_dirty())
    {
        _setDefaults();
    }
    _load();
    _radioConfig.spi_bus = _spi;
    _store();
}

void Config::_setDefaults()
{
    _radioConfig.frequency = 0.0;
    _radioConfig.spreading = 0;
    _radioConfig.signal_bw = 0;
    _radioConfig.coding_rate = 0;
    _radioConfig.cs = TTGO_LORA_CS;
    _radioConfig.dio0 = TTGO_LORA_DIO0;
    _radioConfig.reset = TTGO_LORA_RST;
    _radioConfig.tx_power = LORA_TX_POWER;

    strcpy(_mqttSettings.server, "");
    _mqttSettings.port = 0;
    strcpy(_mqttSettings.user, "");
    strcpy(_mqttSettings.pass, "");
    strcpy(_mqttSettings.id, "");
    strcpy(_mqttSettings.topic, "");
    _dirty(false);
    _store();
}

bool Config::_dirty()
{
    return EEPROM.read(EEPROM_DIRTY_ADDR);
}
void Config::_dirty(bool dirty)
{
    EEPROM.write(EEPROM_DIRTY_ADDR, dirty);
    EEPROM.commit();
}

void Config::_store()
{
    EEPROM.put(EEPROM_LORA_CONFIG_ADDR, _radioConfig);
    EEPROM.put(EEPROM_MQTT_CONFIG_ADDR, _mqttSettings);
    EEPROM.commit();
}

void Config::_load()
{
    EEPROM.get(EEPROM_LORA_CONFIG_ADDR, _radioConfig);
    EEPROM.get(EEPROM_MQTT_CONFIG_ADDR, _mqttSettings);
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

bool Config::setServer(const char *server)
{
    if (strcmp(server, _mqttSettings.server) == 0)
    {
        return true;
    }
    _mqttConfigApplied = false;
    strcpy(_mqttSettings.server, server);
    _store();
    return true;
}
bool Config::setPort(uint16_t port)
{
    if (port == _mqttSettings.port)
    {
        return true;
    }
    _mqttConfigApplied = false;
    _mqttSettings.port = port;
    _store();
    return true;
}
bool Config::setUser(const char *user)
{
    if (strcmp(user, _mqttSettings.user) == 0)
    {
        return true;
    }
    _mqttConfigApplied = false;
    strcpy(_mqttSettings.user, user);
    _store();
    return true;
}
bool Config::setPass(const char *pass)
{
    if (strcmp(pass, _mqttSettings.pass) == 0)
    {
        return true;
    }
    _mqttConfigApplied = false;
    strcpy(_mqttSettings.pass, pass);
    _store();
    return true;
}
bool Config::setId(const char *id)
{
    if (strcmp(id, _mqttSettings.id) == 0)
    {
        return true;
    }
    _mqttConfigApplied = false;
    if (strcmp(id, "") == 0)
    {
        char id[128];
        _mqtt->generateId(id);
        strcpy(_mqttSettings.id, id);
        _store();
        return true;
    }
    strcpy(_mqttSettings.id, id);
    _store();
    return true;
}
bool Config::setTopic(const char *topic)
{
    if (strcmp(topic, _mqttSettings.topic) == 0)
    {
        return true;
    }
    _mqttConfigApplied = false;
    strcpy(_mqttSettings.topic, topic);
    if (_mqtt->setTopic(topic) != MqttClient::MQTT_OK)
    {
        return false;
    }
    _store();
    return true;
}

void Config::getServer(char *server)
{
    strcpy(server, _mqttSettings.server);
}
uint16_t Config::getPort()
{
    return _mqttSettings.port;
}
void Config::getUser(char *user)
{
    strcpy(user, _mqttSettings.user);
}
void Config::getPass(char *pass)
{
    strcpy(pass, _mqttSettings.pass);
}
void Config::getId(char *id)
{
    strcpy(id, _mqttSettings.id);
}
void Config::getTopic(char *topic)
{
    strcpy(topic, _mqttSettings.topic);
}
MqttClient::MqttSettings Config::mqttSettings()
{
    return _mqttSettings;
}
bool Config::configStatus()
{
    return _mqttConfigApplied;
}