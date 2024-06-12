#include "main.h"

Radio radio;
Config config(&radio, &SPI);
Display display;

void setup()
{
  ///< Initialize the serial port
  Serial.begin(SERIAL_SPEED);

  ///< Initialize the I2C bus with correct pins
  Wire.begin(OLED_SDA, OLED_SCL);

  ///< Initialize the status LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  initWIFI(WIFI_SSID, WIFI_PASS);

  ///< Initialize the radio module
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);
  radio.begin(config.radioConfig());

  display.begin();
}

void loop()
{
  display.update();
}