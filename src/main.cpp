#include "main.h"
Radio radio;
Config config(&radio, &SPI);
Display display;
WiFiClient espClient;
SPIClass SPI2(HSPI);
Logger logger(SPI2);
RadioPacket incomingPacket(&logger);
void setup()
{
  ///< Initialize the serial port
  Serial.begin(SERIAL_SPEED);
  Serial.println("Starting...");

  ///< Initialize the system configuration
  config.begin();

  ///< Initialize the I2C bus with correct pins
  Wire.begin(TTGO_OLED_SDA, TTGO_OLED_SCL);

  ///< Initialize the status LED
  pinMode(TTGO_LED_PIN, OUTPUT);

  initWIFI(WIFI_SSID, WIFI_PASS);

  ///< Initialize the radio module
  SPI.begin(TTGO_LORA_SCLK, TTGO_LORA_MISO, TTGO_LORA_MOSI);
  radio.begin(config.radioConfig());

  display.begin();

  SPI2.begin(TTGO_SDCARD_SCLK, TTGO_SDCARD_MISO, TTGO_SDCARD_MOSI, TTGO_SDCARD_CS);
  logger.begin();
}
uint64_t amogus = 0;
void loop()
{
  display.update(logger.status());
  if (radio.receive_bytes())
  {
    incomingPacket.received(radio.received_data.bytes, radio.received_data.length);
    display.update(incomingPacket.lastPacket());
  }
  logger.loop();
}