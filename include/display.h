#pragma once

// ======================== Library Includes ========================
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "radio.h"

enum class FOOTER_WIFI_CONNECTED
{
    IP,
    RSSI,
    SD_STATUS,
};

enum class FOOTER_WIFI_DISCONNECTED
{
    SSID,
    PASSWORD,
    MESSAGE,
    SD_STATUS,
};

struct wifiStatus
{
    bool connected;
    uint8_t ip[4];
    int8_t rssi;
};

struct displayData
{
    float latitude;
    float longitude;
    float altitude;
    float verticalSpeed;
    uint64_t lastPacketTime;
    uint64_t pid;
    uint64_t txid;

    uint8_t chcksumErr;
    uint8_t packetRssi;

    wifiStatus wifi;

    FOOTER_WIFI_CONNECTED footerWifiConnected;
    FOOTER_WIFI_DISCONNECTED footerWifiDisconnected;

    Logger::SD_STATUS sdStatus;
};

class Display
{
private:
    /* data */
    Adafruit_SSD1306 _hwDisplay;
    void _beginOled();
    uint64_t _startTime;
    uint64_t _lastUpdate;
    uint64_t _lastWifiUpdate;
    void _updateDisplay();
    void _converTime(uint64_t time, char *buffer);
    void _getWifiStatus();
    void _updateFooter(char *buffer);
    displayData _data;

public:
    Display(/* args */);
    ~Display();
    void begin();
    void update(Logger::SD_STATUS status);
    void update(RadioPacket::TelemetryData data);
};