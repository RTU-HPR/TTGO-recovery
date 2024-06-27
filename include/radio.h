#pragma once
#include "config.h"

const String LOCATION_FORMAT[] = {"float", "float", "uint16", "uint16", "uint8"};
#define LOCATION_PACKET_DATA_COUNT 5
#define HELLO_radio

class RadioPacket
{
public:
    enum PacketStatus
    {
        PACKET_OK,
        CHECKSUM_ERROR,
    };

    struct TelemetryData
    {
        float latitude;
        float longitude;
        uint16_t baroAltitude;
        uint16_t GPSAltitude;
        uint8_t satellites;
        uint32_t pid;
        uint16_t txid;
    };

private:
    /* data */
    TelemetryData _lastPacket;

public:
    RadioPacket(/* args */);
    ~RadioPacket();

    PacketStatus received(uint8_t *data, uint8_t len);
    TelemetryData lastPacket();
};