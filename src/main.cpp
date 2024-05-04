#include "main.h"

uint64_t oledTick = 0, wifiStatusUpdateTick = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
AsyncWebServer server(WEBSERVER_PORT);

oledData screenData = {
    .latitude = 0.0,
    .longitude = 0.0,
    .altitude = 0,
    .lastPacket = 0,
    .pid = 0,
    .txid = 0,
    .chcksumErr = 0,
    .packetRssi = 0,
};
wifiLoraConfig loraWebConfig = {
    .frequency = 433.0,
    .spreadingFactor = 7,
    .bandwidth = 125,
    .codingRate = 5};

void setup()
{
  ///< Initialize the config
  EEPROM.begin(EEPROM_SIZE);
  loadConfig(&loraWebConfig);
  
  ///< Initialize the serial port
  Serial.begin(SERIAL_SPEED);

  ///< Initialize the I2C bus with correct pins
  Wire.begin(OLED_SDA, OLED_SCL);

  ///< Initialize the status LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  ///< Initialize the OLED display
  beginDisplay();

  initWIFI(WIFI_SSID, WIFI_PASS);
}

void loop()
{
  if (millis() - oledTick > OLED_UPDATE_PERIOD)
  {
    oledTick = millis();
    digitalWrite(25, !digitalRead(25));
    // Serial.println("Hello World");
    updateDisplay(&screenData);
  }

  if (millis() - wifiStatusUpdateTick > WIFI_STATS_UPDATE_PERIOD)
  {
    wifiStatusUpdateTick = millis();
    handleWiFiStats();
  }
}