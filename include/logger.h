#pragma once

// ======================== Library Includes ========================
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include "config.h"

#define DEFAULT_LOG_FILE "/TTGO_LOG_%d.txt"
#define HEADER_FW VERSION "; Compiled on: " COMPILE_DATE
#define HEADER_FORMATING "TTGO Recovery | Firmware: %s | Time ON: %lums\n"
#define REMOUNTED_MSG_FORMATING "Remount Succesfull | Time ON: %lums\n"

class Logger
{
public:
    enum LOG_STATUS
    {
        LOG_OK,
        LOG_FILE_ERROR,
        LOG_CARD_ERROR,
        LOG_FORMAT_ERROR
    };
    enum SD_STATUS
    {
        SD_MOUNTED,
        SD_ERROR
    };

private:
    /* data */

    SPIClass &_spi;
    uint64_t _lastRemount;
    char _fileName[128] = "";

    LOG_STATUS _checkCardMounted();
    LOG_STATUS _tryInitCard();
    LOG_STATUS _generateFileName();
    LOG_STATUS _createLogFile();
    LOG_STATUS _log(char *data);

    bool _isLogFile(char *name);

    SD_STATUS _mounted; /// THIS VARIABLE IS ONLY UPDATED IN LOOP FUNCTION.
                        /// IT MAY NOT REPRESENT THE UP-TO DATE STATUS.

public:
    Logger(SPIClass &spi);
    ~Logger();
    void loop();
    void begin();
    LOG_STATUS log(char *data);
    LOG_STATUS log(String data);
    LOG_STATUS log(char *format, ...);
    SD_STATUS status();
};
