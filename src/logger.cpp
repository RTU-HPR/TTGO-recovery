#include "logger.h"
Logger::Logger(SPIClass &spi) : _spi(spi)
{
    _lastRemount = 0;
}

Logger::~Logger()
{
    if (_checkCardMounted())
    {
        SD.end();
    }
}

void Logger::begin()
{
    _tryInitCard();
    if (_checkCardMounted() == LOG_STATUS::LOG_OK)
    {
        _createLogFile();
    }
}

Logger::LOG_STATUS Logger::_checkCardMounted()
{
    if (!SD.open("/"))
    {
        SD.end();
        return LOG_STATUS::LOG_CARD_ERROR;
    }
    return LOG_STATUS::LOG_OK;
}

Logger::LOG_STATUS Logger::_tryInitCard()
{
    return (SD.begin(TTGO_SDCARD_CS, _spi)) ? LOG_STATUS::LOG_OK : LOG_STATUS::LOG_CARD_ERROR;
}

void Logger::loop()
{
    if (millis() - _lastRemount > LOGGER_REMOUNT_PERIOD)
    {
        _lastRemount = millis();
        if (_checkCardMounted() != LOG_STATUS::LOG_OK)
        {
            _tryInitCard();
            if (_checkCardMounted() == LOG_STATUS::LOG_OK)
            {
                if (strlen(_fileName) == 0 || !SD.exists(_fileName))
                {
                    _createLogFile();
                }
                else
                {
                    char buffer[256];
                    sprintf(buffer, REMOUNTED_MSG_FORMATING, millis());
                    _log(buffer);
                }
                _mounted = SD_STATUS::SD_MOUNTED;
            }
            else
            {
                _mounted = SD_STATUS::SD_ERROR;
            }
        }
    }
}

Logger::LOG_STATUS Logger::_generateFileName()
{
    if (_checkCardMounted() != LOG_STATUS::LOG_OK)
    {
        return LOG_STATUS::LOG_CARD_ERROR;
    }

    uint32_t lastLog = 0;
    File root = SD.open("/");
    if (!root)
    {
        return LOG_STATUS::LOG_FILE_ERROR;
    }

    char nameBuffer[256];
    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            file = root.openNextFile();
            continue;
        }
        strcpy(nameBuffer, file.name());

        if (_isLogFile(nameBuffer))
        {
            int32_t logNumber = atoi(nameBuffer + 9);
            if (logNumber > lastLog)
            {
                lastLog = logNumber;
            }
        }
        file = root.openNextFile();
    }
    sprintf(_fileName, DEFAULT_LOG_FILE, lastLog + 1);
    return LOG_STATUS::LOG_OK;
}

bool Logger::_isLogFile(char *name)
{
    if (strlen(name) < 13)
    {
        return false;
    }
    if (name[0] != 'T' || name[1] != 'T' || name[2] != 'G' || name[3] != 'O' || name[4] != '_' || name[5] != 'L' || name[6] != 'O' || name[7] != 'G' || name[8] != '_')
    {
        return false;
    }
    if (name[strlen(name) - 4] != '.' || name[strlen(name) - 3] != 't' || name[strlen(name) - 2] != 'x' || name[strlen(name) - 1] != 't')
    {
        return false;
    }
    return true;
}

Logger::LOG_STATUS Logger::_createLogFile()
{
    if (_checkCardMounted() != LOG_STATUS::LOG_OK)
    {
        return LOG_STATUS::LOG_CARD_ERROR;
    }

    if (_generateFileName() != LOG_STATUS::LOG_OK)
    {
        return LOG_STATUS::LOG_FILE_ERROR;
    }

    File file = SD.open(_fileName, FILE_WRITE);
    if (!file)
    {
        return LOG_STATUS::LOG_FILE_ERROR;
    }

    char header[1024];
    sprintf(header, HEADER_FORMATING, HEADER_FW, millis());
    file.write((const uint8_t *)header, strlen(header));

    file.close();
    return LOG_STATUS::LOG_OK;
}

Logger::LOG_STATUS Logger::_log(char *data)
{
    if (_checkCardMounted() != LOG_STATUS::LOG_OK)
    {
        return LOG_STATUS::LOG_CARD_ERROR;
    }

    File file = SD.open(_fileName, FILE_APPEND);
    if (!file)
    {
        return LOG_STATUS::LOG_FILE_ERROR;
    }

    file.write((const uint8_t *)data, strlen(data));
    file.close();
    return LOG_STATUS::LOG_OK;
}

Logger::LOG_STATUS Logger::log(char *data)
{
    return _log(data);
}

Logger::LOG_STATUS Logger::log(String data)
{
    return _log((char *)data.c_str());
}

Logger::LOG_STATUS Logger::log(char *format, ...)
{
    size_t bufferSize = 1024;
    char buffer[bufferSize];
    va_list args;

    // Start variadic arguments processing
    va_start(args, format);
    // Try to print to the buffer, returns the number of characters that would have been written if enough space had been available
    int needed = vsnprintf(buffer, bufferSize, format, args);
    va_end(args);

    if (needed < 0)
    {
        return LOG_STATUS::LOG_FORMAT_ERROR; // Formatting error
    }

    if (needed >= bufferSize)
    {
        // The buffer was not large enough
        // Allocate a new buffer with the correct size and print again
        char *dynamicBuffer = new char[needed + 1]; // +1 for null terminator
        va_start(args, format);
        vsnprintf(dynamicBuffer, needed + 1, format, args);
        va_end(args);

        // Log the message and free the dynamically allocated buffer
        LOG_STATUS result = _log(dynamicBuffer);
        delete[] dynamicBuffer;
        return result;
    }
    else
    {
        // The buffer was large enough, log the message
        return _log(buffer);
    }
}

Logger::SD_STATUS Logger::status()
{
    return _mounted;
}