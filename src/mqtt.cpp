#include "mqtt.h"
#include "cert.h"

MqttClient::MqttClient(const char *server, uint16_t port, const char *user, const char *pass, const char *id, const char *topic) : _port(port), _client(_wifiClient)
{
    strcpy(_server, server);
    strcpy(_user, user);
    strcpy(_pass, pass);
    strcpy(_id, id);
    strcpy(_topic, topic);

    if (strcmp(_id, MQTT_ID_DEFAULT) == 0)
    {
        randomSeed(analogRead(0));
        uint16_t id = random(10000, UINT16_MAX);
        sprintf(_id, TTGO_ID_FORMAT, id);
    }

    _lastReconnect = millis();
}

MqttClient::MqttClient() : _port(MQTT_PORT), _client(_wifiClient)
{
    strcpy(_server, MQTT_SERVER);
    strcpy(_user, MQTT_USER);
    strcpy(_pass, MQTT_PASS);
    strcpy(_id, MQTT_ID);
    strcpy(_topic, MQTT_TOPIC);

    if (strcmp(_id, MQTT_ID_DEFAULT) == 0)
    {
        randomSeed(analogRead(0));
        uint16_t id = random(10000, UINT16_MAX);
        sprintf(_id, TTGO_ID_FORMAT, id);
    }

    _lastReconnect = millis();
}

MqttClient::~MqttClient()
{
}

MqttClient::MQTT_STATUS MqttClient::begin()
{
    _wifiClient.setCACert(MQTT_ROOT_CERTIFICATE);
    _client.setClient(_wifiClient);
    _client.setServer(_server, _port);
    return _connect();
}

MqttClient::MQTT_STATUS MqttClient::_connect()
{
    if (_verifySettings() != MQTT_OK)
    {
        return MQTT_DEFAULT_CREDS;
    }
    _client.setServer(_server, _port);
    if (!_client.connect(_id, _user, _pass))
    {
        return MQTT_ERROR;
    }
    return MQTT_OK;
}

MqttClient::MQTT_STATUS MqttClient::_connected()
{
    if (!_client.connected())
    {
        return MQTT_ERROR;
    }
    return MQTT_OK;
}

MqttClient::MQTT_STATUS MqttClient::_verifySettings()
{
    if (strcmp(_server, MQTT_SERVER_DEFAULT) == 0 || strcmp(_user, MQTT_USER_DEFAULT) == 0 || strcmp(_pass, MQTT_PASS_DEFAULT) == 0 || _port == MQTT_PORT_DEAFAULT)
    {
        return MQTT_DEFAULT_CREDS;
    }
    return MQTT_OK;
}

MqttClient::MQTT_STATUS MqttClient::loop()
{
    if (_connected() != MQTT_OK)
    {
        if (millis() - _lastReconnect > MQTT_RECONNECT_PERIOD)
        {
            _lastReconnect = millis();
            return _connect();
        }
        return MQTT_ERROR;
    }
    _client.loop();
    return MQTT_OK;
}

MqttClient::MQTT_STATUS MqttClient::publish(const char *payload)
{
    if (_connected() != MQTT_OK)
    {
        return MQTT_ERROR;
    }
    if (!_client.publish(_topic, payload))
    {
        return MQTT_ERROR;
    }
    return MQTT_OK;
}

MqttClient::MQTT_STATUS MqttClient::publish(const char *payload, const char *topic)
{
    if (_connected() != MQTT_OK)
    {
        return MQTT_ERROR;
    }
    if (!_client.publish(topic, payload))
    {
        return MQTT_ERROR;
    }
    return MQTT_OK;
}

MqttClient::MQTT_STATUS MqttClient::_configure()
{
    if (_connected() == MQTT_OK)
    {
        _client.disconnect();
    }
    return _connect();
}

MqttClient::MQTT_STATUS MqttClient::setServer(const char *server)
{
    strcpy(_server, server);
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setPort(uint16_t port)
{
    _port = port;
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setUser(const char *user)
{
    strcpy(_user, user);
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setPass(const char *pass)
{
    strcpy(_pass, pass);
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setId(const char *id)
{
    strcpy(_id, id);
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setTopic(const char *topic)
{
    strcpy(_topic, topic);
    return _configure();
}

void MqttClient::getServer(char *server)
{
    strcpy(server, _server);
}

uint16_t MqttClient::getPort()
{
    return _port;
}

void MqttClient::getUser(char *user)
{
    strcpy(user, _user);
}

void MqttClient::getPass(char *pass)
{
    strcpy(pass, _pass);
}

void MqttClient::getId(char *id)
{
    strcpy(id, _id);
}

void MqttClient::getTopic(char *topic)
{
    strcpy(topic, _topic);
}

MqttClient::MQTT_STATUS MqttClient::status()
{   
    if(_verifySettings() != MQTT_OK)
    {
        return MQTT_DEFAULT_CREDS;
    }
    return _connected();
}