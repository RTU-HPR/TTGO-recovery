#pragma once
#include "main.h"
#include "teamLogo.h"

enum DISPLAY_WIFI_CONNECTED_STATES
{
    IP,
    RSSI,
};

enum DISPLAY_WIFI_DISCONNECTED_STATES
{
    SSID,
    PASSWORD,
    MESSAGE,
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
    uint64_t lastPacket;
    uint64_t pid;
    uint64_t txid;

    uint8_t chcksumErr;
    uint8_t packetRssi;

    wifiStatus wifi;

    DISPLAY_WIFI_CONNECTED_STATES displayWifiConnectedState;
    DISPLAY_WIFI_DISCONNECTED_STATES displayWifiDisconnectedState;
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
    void _convertWifiStatus(char *buffer);
    displayData _data;

public:
    Display(/* args */);
    ~Display();
    void begin();
    void update();
};