#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

struct oledData
{
    float latitude;
    float longitude;
    float altitude;
    float verticalSpeed;
    float lastPacket;

    uint64_t pid;
    uint64_t txid;

    uint8_t checksumOk;
    uint8_t wifiConnected;
    uint8_t wifiIp[4];
    uint8_t wifiRssi;
    uint8_t packetRssi;
};

struct wifiLoraConfig
{
    float frequency;
    uint8_t spreadingFactor;
    uint16_t bandwidth;
    uint8_t codingRate;
};

#endif // TELEMETRY_H