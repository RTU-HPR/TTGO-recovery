#pragma once

// ======================== Library Includes ========================
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include "config.h"
#include "systemConfig.h"
#include "display.h"
#include "logger.h"

// ======================== Function declarations ========================
void initWIFI(char *wifiSSID, char *wifiPassword);