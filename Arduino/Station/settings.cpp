// File to pull settings from EEPROM
#include "Arduino.h"

#include "settings.h"

#include "utility.h"

#include "EEPROM.h"

// EEPROM length is 1024 bytes, 0x400

#define STATION_META_ADDR 0x020
#define STATION_CONFIG_LEN sizeof(station_meta)
#define STATION_CONFIG_ADDR 0x040
#define STATION_CONFIG_LEN sizeof(station_config)
#define STATION_STATE_ADDR 0x200
#define STATION_CONFIG_LEN sizeof(station_state)

boolean load_settings(station_settings *settings)
{
    EEPROM.get(STATION_META_ADDR, settings->meta);
    if(settings->meta.valid != EEPROM_SETTINGS_VALID){
        // Error
        return false;
    }
    if(strcmp(settings->meta.fw_version, FW_VERSION) != 0){
        // Error
        return false;
    }
    EEPROM.get(STATION_CONFIG_ADDR, settings->config);
    EEPROM.get(STATION_STATE_ADDR, settings->state);
    return true;
}

void save_config(station_config *config)
{
    EEPROM.put(STATION_CONFIG_ADDR, *config);
}


void save_settings(station_settings *settings)
{
    // Write settings to EEPROM
    settings->meta.valid = EEPROM_SETTINGS_VALID;
    EEPROM.put(STATION_META_ADDR, settings->meta);
    EEPROM.put(STATION_CONFIG_ADDR, settings->config);
    EEPROM.put(STATION_STATE_ADDR, settings->state);
}
