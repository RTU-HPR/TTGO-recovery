#pragma once

// ======================== Library Includes ========================
#include <Arduino.h>
#include <EEPROM.h>
#include <MUFFINS_Radio.h>
#include "mqtt.h"
#include "config.h"

#define EEPROM_SIZE 1024                                  ///< EEPROM size in bytes
#define EEPROM_LORA_CONFIG_ADDR 0                         ///< EEPROM address for LoRa configuration
#define EEPROM_MQTT_CONFIG_ADDR sizeof(Radio::Config) + 1 ///< EEPROM address for MQTT configuration
#define EEPROM_DIRTY_ADDR sizeof(Radio::Config) + sizeof(MqttClient::MqttSettings) + 2 ///< EEPROM address for dirty flag

class Config
{
private:
    Radio::Config _radioConfig;
    MqttClient::MqttSettings _mqttSettings;
    Radio *_radio;
    SPIClass *_spi;
    MqttClient *_mqtt;

    bool _mqttConfigApplied = true;

    void _store();
    void _load();
    void _setDefaults();
    bool _dirty();
    void _dirty(bool dirty);   

public:
    Config(Radio *radio, MqttClient *mqtt, SPIClass *spi);
    ~Config();

    void begin();
    bool configStatus();

    float frequency();
    uint8_t spreadingFactor();
    float bandwidth();
    uint8_t codingRate();
    Radio::Config radioConfig();
    bool frequency(float frequency);
    bool spreadingFactor(uint8_t spreadingFactor);
    bool bandwidth(float bandwidth);
    bool codingRate(uint8_t codingRate);
    

    bool setServer(const char *server);
    bool setPort(uint16_t port);
    bool setUser(const char *user);
    bool setPass(const char *pass);
    bool setId(const char *id);
    bool setTopic(const char *topic);
    void getServer(char *server);
    uint16_t getPort();
    void getUser(char *user);
    void getPass(char *pass);
    void getId(char *id);
    void getTopic(char *topic);
    MqttClient::MqttSettings mqttSettings();
};
