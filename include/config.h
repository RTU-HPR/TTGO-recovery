#pragma once

#include "libs.h"
#include "TTGOPins.h"

// ======================== Constants ========================
#define SERIAL_SPEED 115200 ///< Serial communication speed

#define LORA_TX_POWER 15 ///< Transmit power in dBm
#define RADIO_MODULE 2   ///< Radio module to be used (SX1278)

#define OLED_UPDATE_FREQ 15                        ///< OLED update frequency in Hz
#define OLED_UPDATE_PERIOD 1000 / OLED_UPDATE_FREQ ///< Update period in ms
#define SCREEN_WIDTH 128                           ///< OLED display width, in pixels
#define SCREEN_HEIGHT 64                           ///< OLED display height, in pixels
#define RTU_HPR_SPLASH_TIMEOUT 0                   ///< Enable HPR splash screen
#define SSD1306_NO_SPLASH                          ///< Dissble AdaFruits Splash logo
#define DISPLAY_ROW_STEP 10                        ///< Row step for display
#define WIFI_STATS_CHANGE_PERIOD 2000              ///< WiFi stats update period in ms

#define WIFI_HOSTNAME "RTU-HPR-Recovery"                          ///< WiFi hostname
#define WIFI_SSID (char *)"RTU_HPR_RECOVERY"                              ///< WiFi SSID
#define WIFI_PASS (char *)"ttgorecovery"                                  ///< WiFi password
#define WEBSERVER_PORT 80                                         ///< Webserver port
#define WIFI_STATS_UPDATE_FREQ 100                                ///< WiFi stats update frequency in mHz
#define WIFI_STATS_UPDATE_PERIOD 1000000 / WIFI_STATS_UPDATE_FREQ ///< WiFi stats update period in ms

#define EEPROM_SIZE 512          ///< EEPROM size in bytes
#define EEPROM_LORA_CONFIG_ADDR 0 ///< EEPROM address for LoRa configuration

// ======================== Function declarations ========================
void initWIFI(char *wifiSSID, char *wifiPassword);