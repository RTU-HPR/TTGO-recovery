#include "main.h"
#include "webpage.h"

extern AsyncWebServer server;
extern wifiLoraConfig loraWebConfig;
extern oledData screenData;

void returnIndex(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", webPage);
}

void returnParams(AsyncWebServerRequest *request)
{
    char buffer[128];
    if (request->url() == "/frequency")
    {
        sprintf(buffer, "%.3f", loraWebConfig.frequency);
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/spreadingFactor")
    {
        sprintf(buffer, "%d", loraWebConfig.spreadingFactor);
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/bandwidth")
    {
        sprintf(buffer, "%d", loraWebConfig.bandwidth);
        request->send(200, "text/plain", buffer);
    }
    else if (request->url() == "/codingRate")
    {
        sprintf(buffer, "4/%d", loraWebConfig.codingRate);
        request->send(200, "text/plain", buffer);
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
        dataBuffer[i] = (char)data[i];
    }
    dataBuffer[len] = '\0';

    if (request->url() == "/frequency")
    {
        loraWebConfig.frequency = atoff((const char *)dataBuffer);
        request->send(200);
    }
    else if (request->url() == "/spreadingFactor")
    {
        loraWebConfig.spreadingFactor = atoi((const char *)dataBuffer);
        request->send(200);
    }
    else if (request->url() == "/bandwidth")
    {
        loraWebConfig.bandwidth = atoi((const char *)dataBuffer);
        request->send(200);
    }
    else if (request->url() == "/codingRate")
    {
        sscanf((const char *)dataBuffer, "4/%d", &loraWebConfig.codingRate);
        request->send(200);
    }
    else
    {
        request->send(404);
    }
}

void initWebServer()
{
    server.on("/", HTTP_GET, returnIndex);

    server.on("/frequency", HTTP_GET, returnParams);
    server.on("/spreadingFactor", HTTP_GET, returnParams);
    server.on("/bandwidth", HTTP_GET, returnParams);
    server.on("/codingRate", HTTP_GET, returnParams);

    server.on(
        "/frequency", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/spreadingFactor", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/bandwidth", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
    server.on(
        "/codingRate", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, setParams);
}

void handleWiFiStats(){
    if (WiFi.status() == WL_CONNECTED)
    {
        screenData.wifi.connected = 1;
        screenData.wifi.rssi = WiFi.RSSI();
        screenData.wifi.ip[0] = WiFi.localIP()[0];
        screenData.wifi.ip[1] = WiFi.localIP()[1];
        screenData.wifi.ip[2] = WiFi.localIP()[2];
        screenData.wifi.ip[3] = WiFi.localIP()[3];
    }
    else
    {
        screenData.wifi.connected = 0;
    }
}

void initWIFI(char *wifiSSID, char *wifiPassword)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPassword);
    WiFi.setHostname("TTGO-Recovery");
    server.begin();
    initWebServer();
}