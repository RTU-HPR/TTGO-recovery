#pragma once

// ======================== Library Includes ========================
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "config.h"

#define MQTT_SERVER_DEFAULT "default"
#define MQTT_PORT_DEAFAULT 0
#define MQTT_USER_DEFAULT "default"
#define MQTT_PASS_DEFAULT "default"
#define MQTT_ID_DEFAULT "random"
#define MQTT_TOPIC_DEFAULT "default"

#ifndef MQTT_SERVER
#define MQTT_SERVER MQTT_SERVER_DEFAULT
#endif
#ifndef MQTT_PORT
#define MQTT_PORT MQTT_PORT_DEAFAULT
#endif
#ifndef MQTT_USER
#define MQTT_USER MQTT_USER_DEFAULT
#endif
#ifndef MQTT_PASS
#define MQTT_PASS MQTT_PASS_DEFAULT
#endif
#ifndef MQTT_ID
#define MQTT_ID MQTT_ID_DEFAULT
#endif
#ifndef MQTT_TOPIC
#define MQTT_TOPIC MQTT_TOPIC_DEFAULT
#endif

#define TTGO_ID_FORMAT "TTGO-RECOVERY-%ld"

#define MQTT_CONNECT_ATTMEPT_PERIOD 100

class MqttClient
{
public:
    enum MQTT_STATUS
    {
        MQTT_OK,
        MQTT_ERROR,
        MQTT_DEFAULT_CREDS
    };

    struct MqttSettings
    {
        char server[128];
        uint16_t port;
        char user[128];
        char pass[128];
        char id[128];
        char topic[128];
    };

private:
    /* data */
    WiFiClientSecure _wifiClient;
    PubSubClient _client;

    char _server[128];
    uint16_t _port;
    char _user[128];
    char _pass[128];
    char _id[128];
    char _topic[128];
    uint64_t _lastReconnectLoop;
    uint64_t _lastConnectionAttempt;

    MQTT_STATUS _connect();
    MQTT_STATUS _connected();
    MQTT_STATUS _verifySettings();
    MQTT_STATUS _configure(bool reconnect = false);

public:
    MqttClient(const char *server, uint16_t port, const char *user, const char *pass, const char *id, const char *topic);
    MqttClient();
    ~MqttClient();

    MQTT_STATUS begin(MqttSettings settings);
    MQTT_STATUS begin();
    MQTT_STATUS loop();
    MQTT_STATUS publish(const char *payload);
    MQTT_STATUS publish(const char *payload, const char *topic);
    MQTT_STATUS status();
    MQTT_STATUS generateId(char *id);

    MQTT_STATUS setServer(const char *server);
    MQTT_STATUS setPort(uint16_t port);
    MQTT_STATUS setUser(const char *user);
    MQTT_STATUS setPass(const char *pass);
    MQTT_STATUS setId(const char *id);
    MQTT_STATUS setTopic(const char *topic);

    void getServer(char *server);
    uint16_t getPort();
    void getUser(char *user);
    void getPass(char *pass);
    void getId(char *id);
    void getTopic(char *topic);
};
