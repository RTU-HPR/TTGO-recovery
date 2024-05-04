
/// Declarations for the OLED display
void beginDisplay();
void updateDisplay(oledData *data);

/// Declarations for the WiFi module
void initWIFI(char *wifiSSID, char *wifiPassword);

/// Declarations for the persistent configuration storage
void storeConfig(wifiLoraConfig *config);
void loadConfig(wifiLoraConfig *config);