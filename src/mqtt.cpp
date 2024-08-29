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
        char buffer[64];
        generateId(buffer);
        strcpy(_id, buffer);
    }

    _lastReconnectLoop = millis();
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
        char buffer[64];
        generateId(buffer);
        strcpy(_id, buffer);
    }

    _lastReconnectLoop = millis();
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

MqttClient::MQTT_STATUS MqttClient::begin(MqttSettings settings)
{
    strcpy(_server, settings.server);
    _port = settings.port;
    strcpy(_user, settings.user);
    strcpy(_pass, settings.pass);
    strcpy(_id, settings.id);
    strcpy(_topic, settings.topic);

    _wifiClient.setCACert(MQTT_ROOT_CERTIFICATE);
    _client.setClient(_wifiClient);
    _client.setServer(_server, _port);
    return _connect();
}

MqttClient::MQTT_STATUS MqttClient::generateId(char *id)
{
    randomSeed(analogRead(0));
    uint16_t numId = random(10000, UINT16_MAX);
    sprintf(id, TTGO_ID_FORMAT, numId);
    return MqttClient::MQTT_OK;
}

MqttClient::MQTT_STATUS MqttClient::_connect()
{
    while (millis() - _lastConnectionAttempt < MQTT_CONNECT_ATTMEPT_PERIOD)
        ;
    _lastConnectionAttempt = millis();

    if (_verifySettings() != MQTT_OK)
    {
        return MQTT_DEFAULT_CREDS;
    }
    if (_connected() == MQTT_OK)
    {
        _client.disconnect();
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
    _client.loop();
    if (_connected() != MQTT_OK)
    {
        if (millis() - _lastReconnectLoop > MQTT_RECONNECT_LOOP_PERIOD)
        {
            _lastReconnectLoop = millis();
            return _connect();
        }
        return MQTT_ERROR;
    }
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

MqttClient::MQTT_STATUS MqttClient::_configure(bool reconnect)
{
    if (_connected() == MQTT_OK)
    {
        _client.disconnect();
    }
    return _connect();
}

MqttClient::MQTT_STATUS MqttClient::setServer(const char *server)
{
    if (strcmp(server, _server) == 0)
    {
        return MQTT_OK;
    }
    strcpy(_server, server);
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setPort(uint16_t port)
{
    if (_port == port)
    {
        return MQTT_OK;
    }
    _port = port;
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setUser(const char *user)
{
    if (strcmp(user, _user) == 0)
    {
        return MQTT_OK;
    }
    strcpy(_user, user);
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setPass(const char *pass)
{
    if (strcmp(pass, _pass) == 0)
    {
        return MQTT_OK;
    }
    strcpy(_pass, pass);
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setId(const char *id)
{
    if (strcmp(_id, id) == 0)
    {
        return MQTT_OK;
    }
    strcpy(_id, id);
    return _configure();
}

MqttClient::MQTT_STATUS MqttClient::setTopic(const char *topic)
{
    if (strcmp(_topic, topic) == 0)
    {
        return MQTT_OK;
    }
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
    if (_verifySettings() != MQTT_OK)
    {
        return MQTT_DEFAULT_CREDS;
    }
    return _connected();
}