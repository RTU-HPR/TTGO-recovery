#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

struct wifiStatus
{
    uint8_t connected;
    uint8_t ip[4];
    int8_t rssi;
};

struct oledData
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
};

struct wifiLoraConfig
{
    float frequency;
    uint8_t spreadingFactor;
    uint16_t bandwidth;
    uint8_t codingRate;
};

#endif // TELEMETRY_H