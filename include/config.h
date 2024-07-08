#pragma once

// ======================== Firmware Version ========================
#define VERSION "0.1pre"               ///< Firmware version
#define COMPILE_DATE __DATE__ __TIME__ ///< Compilation date

// ======================== General Configuration ========================
#define SERIAL_SPEED 115200 ///< Serial communication speed

// ======================== Radio Configuration ========================
#define LORA_TX_POWER 15 ///< Transmit power in dBm
#define RADIO_MODULE 2   ///< Radio module to be used (SX1278)

// ======================== Display Configuration ========================
#define OLED_UPDATE_FREQ 15             ///< OLED update frequency in Hz
#define RTU_HPR_SPLASH_TIMEOUT 0        ///< Enable HPR splash screen timeout in ms
#define FOOTER_STATS_CHANGE_PERIOD 2000 ///< WiFi stats update period in ms

// ======================== WiFi Configuration ========================
#define WIFI_HOSTNAME (char *)"RTU_HPR_RECOVERY" ///< WiFi hostname
#define WIFI_SSID (char *)"RTU_HPR_RECOVERY"     ///< WiFi SSID
#define WIFI_PASS (char *)"ttgorecovery"         ///< WiFi password
#define WEBSERVER_PORT 80                        ///< Webserver port

// ======================== SD Card Logger Configuration ========================
#define LOGGER_REMOUNT_PERIOD 5000 ///< SD card remount period in ms

// ======================== MQTT Configuration ========================
// #define MQTT_SERVER                                                  ///< MQTT server address
// #define MQTT_PORT                                                    ///< MQTT server port
// #define MQTT_USER                                                    ///< MQTT username
// #define MQTT_PASS                                                    ///< MQTT password
// #define MQTT_ID                                                      ///< MQTT client ID ("random" for random ID)
// #define MQTT_TOPIC                                                   ///< MQTT topic

#include "TTGOPins.h"