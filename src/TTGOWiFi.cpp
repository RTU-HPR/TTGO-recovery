#include "TTGOWiFi.h"
AsyncWebServer server(WEBSERVER_PORT);
extern Config config;

void redirectIndex(AsyncWebServerRequest *request)
{
    request->redirect("/radio.html");
}
void returnRadioSettings(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", RADIO_SETTINGS_PAGE);
}
void returnMqttSettings(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", MQTT_SETTINGS_PAGE);
}

void applySettings(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{   
    request->send(200, "text/plain", "applied");
    ESP.restart();
}

void returnParams(AsyncWebServerRequest *request)
{
    char buffer[128];
    if (request->url() == "/frequency")
    {
        sprintf(buffer, "%.3f", config.frequency());
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/spreadingFactor")
    {
        sprintf(buffer, "%d", config.spreadingFactor());
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/bandwidth")
    {
        sprintf(buffer, "%.2f", config.bandwidth());
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/codingRate")
    {
        sprintf(buffer, "4/%d", config.codingRate());
        request->send(200, "text/plain", buffer);
    }

    else if (request->url() == "/serverDomain")
    {
        config.getServer(buffer);
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/username")
    {
        config.getUser(buffer);
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/password")
    {
        config.getPass(buffer);
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/topic")
    {
        config.getTopic(buffer);
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/port")
    {
        sprintf(buffer, "%d", config.getPort());
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/id")
    {
        config.getId(buffer);
        request->send(200, "text/plain", buffer);
    }

    else if (request->url() == "/settingsStatus")
    {
        bool status = config.configStatus();
        request->send(200, "text/plain", status ? "1" : "0");
    }

    else
    {
        request->send(404);
    }
}

void setParams(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // Converting incoming data to cstring
    char dataBuffer[len + 1];
    for (int i = 0; i < len; i++)
    {
        dataBuffer[i] = static_cast<char>(data[i]);
    }
    dataBuffer[len] = '\0';

    if (request->url() == "/frequency")
    {
        config.frequency(static_cast<float>(atoff((const char *)dataBuffer)));
        request->send(200);
    }
    else if (request->url() == "/spreadingFactor")
    {
        config.spreadingFactor(static_cast<float>(atoff((const char *)dataBuffer)));
        request->send(200);
    }
    else if (request->url() == "/bandwidth")
    {
        config.bandwidth(static_cast<float>(atoff((const char *)dataBuffer)));
        request->send(200);
    }
    else if (request->url() == "/codingRate")
    {
        config.codingRate(static_cast<float>(atoff((const char *)dataBuffer)));
        request->send(200);
    }

    else if (request->url() == "/serverDomain")
    {
        config.setServer(dataBuffer);
        request->send(200);
    }
    else if (request->url() == "/username")
    {
        config.setUser(dataBuffer);
        request->send(200);
    }
    else if (request->url() == "/password")
    {
        config.setPass(dataBuffer);
        request->send(200);
    }
    else if (request->url() == "/topic")
    {
        config.setTopic(dataBuffer);
        request->send(200);
    }
    else if (request->url() == "/port")
    {
        config.setPort(static_cast<uint16_t>(atoi(dataBuffer)));
        request->send(200);
    }
    else if (request->url() == "/id")
    {
        config.setId(dataBuffer);
        request->send(200);
    }
    else if (request->url() == "/generateid")
    {
        config.setId("");
        config.getId(dataBuffer);
        request->send(200, "text/plain", dataBuffer);
    }
    else
    {
        request->send(404);
        return;
    }
}

void initWebServer()
{
    server.on("/", HTTP_GET, redirectIndex);
    server.on("/radio.html", HTTP_GET, returnRadioSettings);
    server.on("/mqtt.html", HTTP_GET, returnMqttSettings);

    server.on("/frequency", HTTP_GET, returnParams);
    server.on("/spreadingFactor", HTTP_GET, returnParams);
    server.on("/bandwidth", HTTP_GET, returnParams);
    server.on("/codingRate", HTTP_GET, returnParams);

    server.on("/serverDomain", HTTP_GET, returnParams);
    server.on("/username", HTTP_GET, returnParams);
    server.on("/password", HTTP_GET, returnParams);
    server.on("/topic", HTTP_GET, returnParams);
    server.on("/port", HTTP_GET, returnParams);
    server.on("/id", HTTP_GET, returnParams);

    server.on(
        "/frequency", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/spreadingFactor", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/bandwidth", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/codingRate", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);

    server.on(
        "/serverDomain", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/username", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/password", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/topic", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/port", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/id", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/generateid", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);

    server.on("/settingsStatus", HTTP_GET, returnParams);
    server.on("/apply", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, applySettings);
}

void initWIFI(char *wifiSSID, char *wifiPassword)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPassword);
    WiFi.setHostname(WIFI_HOSTNAME);
    server.begin();
    initWebServer();
}