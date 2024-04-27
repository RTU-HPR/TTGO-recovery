#include "main.h"

uint64_t oledTick = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
AsyncWebServer server(WEBSERVER_PORT);

oledData screenData;
wifiLoraConfig loraWebConfig = {
    .frequency = 868.0,
    .spreadingFactor = 7,
    .bandwidth = 125,
    .codingRate = 5};

void setup()
{
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
    //Serial.println("Hello World");
    updateDisplay(&screenData);
  }
}