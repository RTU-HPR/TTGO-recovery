#include "main.h"

extern Adafruit_SSD1306 display;
extern uint64_t oledTick;
uint32_t startTime = 0;
uint64_t wifiStatsTick = 0;
uint8_t wifiStats = 0;

void beginDisplay()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        while (1)
            ;
    }
    display.clearDisplay();
    display.display();
    display.drawBitmap(32, 0, HPR_LOGO, 64, 64, WHITE);
    display.display();
    oledTick = millis();
    startTime = millis();
}

void updateDisplay(oledData *data)
{
    static char buffer[128];
    static char scrollBuffer[256];
    if (millis() - startTime > RTU_HPR_SPLASH_TIMEOUT)
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);

        sprintf(buffer, "LAT:%.6f", data->latitude);
        display.setCursor(0, 0);
        display.println(buffer);

        sprintf(buffer, "LON:%.6f", data->longitude);
        display.setCursor(0, DISPLAY_ROW_STEP);
        display.println(buffer);

        sprintf(buffer, "ALT:%.0fm", data->altitude);
        display.setCursor(0, DISPLAY_ROW_STEP * 2);
        display.println(buffer);

        sprintf(buffer, "LST:%.1fs", data->lastPacket);
        display.setCursor(66, DISPLAY_ROW_STEP * 2);
        display.println(buffer);

        sprintf(buffer, "PID:%lu", data->pid);
        display.setCursor(0, DISPLAY_ROW_STEP * 3);
        display.println(buffer);

        sprintf(buffer, "TXID:%lu", data->txid);
        display.setCursor(72, DISPLAY_ROW_STEP * 3);
        display.println(buffer);

        sprintf(buffer, "RSSI:%u", data->packetRssi);
        display.setCursor(0, DISPLAY_ROW_STEP * 4);
        display.println(buffer);

        if (data->checksumOk)
            sprintf(buffer, "CHCKSUM:OK");
        else
            sprintf(buffer, "CHCKSUM:ERR");
        display.setCursor(54, DISPLAY_ROW_STEP * 4);
        display.println(buffer);

        display.drawLine(0, 50, 128, 50, WHITE);

        if (data->wifiConnected)
        {
            if(millis() - wifiStatsTick > WIFI_STATS_CHANGE_PERIOD)
            {
                wifiStatsTick = millis();
                wifiStats = ~wifiStats & 0x01;
            }   
        }
        else
        {
            sprintf(buffer, "WIFI:DISCONNECTED");
        }
        display.setCursor(0, 54);
        display.println(buffer);

        display.display();
    }
}