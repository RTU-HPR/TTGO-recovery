#include "radio.h"

RadioPacket::RadioPacket(Logger *logger, MqttClient *mqtt) : _logger(logger), _mqtt(mqtt)
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
    _lastPacket.epochTime = epochTime;

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

    _log(&_lastPacket);///< Log the data
    _publish(&_lastPacket);///< Publish the data

    delete dataBuffer;
    delete buffer;
    return PACKET_OK;
}

RadioPacket::TelemetryData RadioPacket::lastPacket()
{
    return _lastPacket;
}

Logger::LOG_STATUS RadioPacket::_log(TelemetryData *data)
{
    return _logger->log((char *)"Time on: %lu Epoch: %lu Latitude: %f Longitude: %f Baro Altitude: %d GPS Altitude: %d Satellites: %d PID: %d TXID: %d\n",
                        millis(), data->epochTime, data->latitude, data->longitude, data->baroAltitude, data->GPSAltitude, data->satellites, data->pid, data->txid);
}

MqttClient::MQTT_STATUS RadioPacket::_publish(TelemetryData *data)
{
    char buffer[512];
    sprintf(buffer, "Time on: %lu Epoch: %lu Latitude: %f Longitude: %f Baro Altitude: %d GPS Altitude: %d Satellites: %d PID: %d TXID: %d\n",
            millis(), data->epochTime, data->latitude, data->longitude, data->baroAltitude, data->GPSAltitude, data->satellites, data->pid, data->txid);
    return _mqtt->publish(buffer);
}