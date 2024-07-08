#include "display.h"

Display::Display(/* args */) : _hwDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire)
{
}

Display::~Display()
{
}

void Display::begin()
{
    _beginOled();
    _startTime = millis();
    _lastUpdate = millis();
}

void Display::_beginOled()
{
    if (!_hwDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        while (1)
            ;
    }
    _hwDisplay.clearDisplay();
    _hwDisplay.display();
    _hwDisplay.drawBitmap(32, 0, HPR_LOGO, 64, 64, WHITE);
    _hwDisplay.display();
}

void Display::_converTime(uint64_t time, char *buffer)
{
    float dt = (millis() - time) / 1000.0;
    if (dt < 60.0)
    {
        sprintf(buffer, "LST:%.1fs", dt);
    }
    else if (dt < 59999.0) /// 999 * 60 + 59
    {
        uint8_t minutes = dt / 60.0;
        uint8_t seconds = fmod(dt, 60.0);
        if (seconds < 10)
            sprintf(buffer, "LST:%u:0%u", minutes, seconds);
        else
            sprintf(buffer, "LST:%u:%u", minutes, seconds);
    }
    else
    {
        sprintf(buffer, "LST:Long");
    }
}

void Display::_getWifiStatus()
{
    _data.wifi.connected = (WiFi.status() == WL_CONNECTED);
    if (_data.wifi.connected)
    {
        _data.wifi.ip[0] = WiFi.localIP()[0];
        _data.wifi.ip[1] = WiFi.localIP()[1];
        _data.wifi.ip[2] = WiFi.localIP()[2];
        _data.wifi.ip[3] = WiFi.localIP()[3];
        _data.wifi.rssi = WiFi.RSSI();
    }
}

void Display::_updateFooter(char *buffer)
{
    _getWifiStatus();
    if (_data.wifi.connected)
    {
        if (_data.footerWifiConnected == FOOTER_WIFI_CONNECTED::IP)
        {
            sprintf(buffer, "IP:%u.%u.%u.%u", _data.wifi.ip[0], _data.wifi.ip[1], _data.wifi.ip[2], _data.wifi.ip[3]);
            if (millis() - _lastWifiUpdate > FOOTER_STATS_CHANGE_PERIOD)
            {
                _lastWifiUpdate = millis();
                _data.footerWifiConnected = FOOTER_WIFI_CONNECTED::RSSI;
            }
        }
        else if (_data.footerWifiConnected == FOOTER_WIFI_CONNECTED::RSSI)
        {
            sprintf(buffer, "WIFI RSSI:%d", _data.wifi.rssi);
            if (millis() - _lastWifiUpdate > FOOTER_STATS_CHANGE_PERIOD)
            {
                _lastWifiUpdate = millis();
                _data.footerWifiConnected = FOOTER_WIFI_CONNECTED::SD_STATUS;
            }
        }
        else
        {
            sprintf(buffer, "SD CARD:%s", _data.sdStatus == Logger::SD_STATUS::SD_MOUNTED ? "OK" : "NOT PRESENT");
            if (millis() - _lastWifiUpdate > FOOTER_STATS_CHANGE_PERIOD)
            {
                _lastWifiUpdate = millis();
                _data.footerWifiConnected = FOOTER_WIFI_CONNECTED::IP;
            }
        }
    }
    else
    {
        if (_data.footerWifiDisconnected == FOOTER_WIFI_DISCONNECTED::SSID)
        {
            sprintf(buffer, "SSID:%s", WIFI_SSID);
            if (millis() - _lastWifiUpdate > FOOTER_STATS_CHANGE_PERIOD)
            {
                _lastWifiUpdate = millis();
                _data.footerWifiDisconnected = FOOTER_WIFI_DISCONNECTED::PASSWORD;
            }
        }
        else if (_data.footerWifiDisconnected == FOOTER_WIFI_DISCONNECTED::PASSWORD)
        {
            sprintf(buffer, "PASS:%s", WIFI_PASS);
            if (millis() - _lastWifiUpdate > FOOTER_STATS_CHANGE_PERIOD)
            {
                _lastWifiUpdate = millis();
                _data.footerWifiDisconnected = FOOTER_WIFI_DISCONNECTED::MESSAGE;
            }
        }
        else if (_data.footerWifiDisconnected == FOOTER_WIFI_DISCONNECTED::MESSAGE)
        {
            sprintf(buffer, "TURN ON WIFI HOTSPOT!");
            if (millis() - _lastWifiUpdate > FOOTER_STATS_CHANGE_PERIOD)
            {
                _lastWifiUpdate = millis();
                _data.footerWifiDisconnected = FOOTER_WIFI_DISCONNECTED::SD_STATUS;
            }
        }
        else
        {
            sprintf(buffer, "SD CARD:%s", _data.sdStatus == Logger::SD_STATUS::SD_MOUNTED ? "OK" : "NOT PRESENT");
            if (millis() - _lastWifiUpdate > FOOTER_STATS_CHANGE_PERIOD)
            {
                _lastWifiUpdate = millis();
                _data.footerWifiDisconnected = FOOTER_WIFI_DISCONNECTED::SSID;
            }
        }
    }
}

void Display::_updateDisplay()
{
    static char buffer[128];
    static char scrollBuffer[256];
    if (!(millis() - _startTime > RTU_HPR_SPLASH_TIMEOUT))
    {
        return;
    }

    _hwDisplay.clearDisplay();
    _hwDisplay.setTextSize(1);
    _hwDisplay.setTextColor(WHITE);

    sprintf(buffer, "LAT:%.6f", _data.latitude);
    _hwDisplay.setCursor(0, 0);
    _hwDisplay.println(buffer);

    sprintf(buffer, "LON:%.6f", _data.longitude);
    _hwDisplay.setCursor(0, DISPLAY_ROW_STEP);
    _hwDisplay.println(buffer);

    sprintf(buffer, "ALT:%.0fm", _data.altitude);
    _hwDisplay.setCursor(0, DISPLAY_ROW_STEP * 2);
    _hwDisplay.println(buffer);

    _converTime(_data.lastPacketTime, buffer);
    _hwDisplay.setCursor(66, DISPLAY_ROW_STEP * 2);
    _hwDisplay.println(buffer);

    sprintf(buffer, "PID:%lu", _data.pid);
    _hwDisplay.setCursor(0, DISPLAY_ROW_STEP * 3);
    _hwDisplay.println(buffer);

    sprintf(buffer, "TXID:%lu", _data.txid);
    _hwDisplay.setCursor(72, DISPLAY_ROW_STEP * 3);
    _hwDisplay.println(buffer);

    sprintf(buffer, "RSSI:%u", _data.packetRssi);
    _hwDisplay.setCursor(0, DISPLAY_ROW_STEP * 4);
    _hwDisplay.println(buffer);

    if (_data.chcksumErr)
        sprintf(buffer, "CHCKSUM:ERR");
    else
        sprintf(buffer, "CHCKSUM:OK");
    _hwDisplay.setCursor(60, DISPLAY_ROW_STEP * 4);
    _hwDisplay.println(buffer);

    _hwDisplay.drawLine(0, 50, 128, 50, WHITE);

    _updateFooter(buffer);
    _hwDisplay.setCursor(0, 54);
    _hwDisplay.println(buffer);

    _hwDisplay.display();
}

void Display::update(Logger::SD_STATUS status)
{
    if (millis() - _lastUpdate > OLED_UPDATE_PERIOD)
    {
        _lastUpdate = millis();
        _updateDisplay();
        _data.sdStatus = status;
    }
}

void Display::update(RadioPacket::TelemetryData data)
{
    _data.latitude = data.latitude;
    _data.longitude = data.longitude;
    _data.altitude = data.baroAltitude;
    _data.pid = data.pid;
    _data.txid = data.txid;
}