#include "radio.h"
#include "MUFFINS_CCSDS_Packets.h"
#include "MUFFINS_Checksums.h"



RadioPacket::RadioPacket(/* args */)
{
}

RadioPacket::~RadioPacket()
{
}

RadioPacket::PacketStatus RadioPacket::received(uint8_t *data, uint8_t len)
{
    ///< Check the packet length
    if (!Checksum_CCITT_16::verify(data, len))
        return CHECKSUM_ERROR;

    ///< Parse the packet
    uint8_t *buffer = new uint8_t[len];
    uint16_t apid, sequenceCount, subseconds, dataLength;
    uint32_t epochTime;
    parse_ccsds_telemetry(data, apid, sequenceCount, epochTime, subseconds, buffer, dataLength);

    _lastPacket.pid = sequenceCount;
    _lastPacket.txid = apid;

    ///< Parse the data
    uint8_t *dataBuffer = new uint8_t[dataLength];
    memcpy(dataBuffer, buffer, dataLength);
    Converter *telemetry = new Converter[LOCATION_PACKET_DATA_COUNT];
    extract_ccsds_data_values(dataBuffer, LOCATION_PACKET_DATA_COUNT, LOCATION_FORMAT, telemetry);

    _lastPacket.latitude = telemetry[0].f;
    _lastPacket.longitude = telemetry[1].f;
    _lastPacket.baroAltitude = telemetry[2].i16;
    _lastPacket.GPSAltitude = telemetry[3].i16;
    _lastPacket.satellites = telemetry[4].i8;

    delete dataBuffer;
    delete buffer;
    return PACKET_OK;
}

RadioPacket::TelemetryData RadioPacket::lastPacket()
{
    return _lastPacket;
}