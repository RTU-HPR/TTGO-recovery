#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MUFFINS_Radio.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

#include "TTGOPins.h"
#include "teamLogo.h"
#include "telemetry.h"
#include "declarations.h"

#define SERIAL_SPEED 115200 ///< Serial communication speed

#define LORA_TX_POWER 22 ///< Transmit power in dBm

#define OLED_UPDATE_FREQ 10                        ///< OLED update frequency in Hz
#define OLED_UPDATE_PERIOD 1000 / OLED_UPDATE_FREQ ///< Update period in ms
#define SCREEN_WIDTH 128                           ///< OLED display width, in pixels
#define SCREEN_HEIGHT 64                           ///< OLED display height, in pixels
#define RTU_HPR_SPLASH_TIMEOUT 0                   ///< Enable HPR splash screen
#define SSD1306_NO_SPLASH                          ///< Dissble AdaFruits Splash logo
#define DISPLAY_ROW_STEP 10                        ///< Row step for display
#define WIFI_STATS_CHANGE_PERIOD 2000              ///< WiFi stats update period in ms

#define WIFI_SSID "RTU_HPR_RECOVERY" ///< WiFi SSID
#define WIFI_PASS "ttgorecovery"     ///< WiFi password
#define WEBSERVER_PORT 80            ///< Webserver port

#define EEPROM_SIZE 512          ///< EEPROM size in bytes
#define EEPROM_LORA_CONFIG_ADDR 0 ///< EEPROM address for LoRa configuration

#endif // MAIN_H