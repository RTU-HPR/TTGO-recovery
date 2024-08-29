#pragma once

// ======================== Library Includes ========================
#include <Arduino.h>
#include "config.h"
#include "teamLogo.h"
#include "logger.h"
#include "MUFFINS_CCSDS_Packets.h"
#include "MUFFINS_Checksums.h"

const String LOCATION_FORMAT[] = {"float", "float", "uint16", "uint16", "uint8"};
#define LOCATION_PACKET_DATA_COUNT 5

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
        uint64_t epochTime;
    };

private:
    /* data */
    TelemetryData _lastPacket;
    Logger *_logger;

    Logger::LOG_STATUS _log(TelemetryData *data);

public:
    RadioPacket(Logger *logger);
    ~RadioPacket();

    PacketStatus received(uint8_t *data, uint8_t len);
    TelemetryData lastPacket();
};