#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Arduino.h"

#define CMD_STATION_TOPIC "cmd/irrigation/tank_station1"

#define CMD_STATION_TOPIC_LENGTH (sizeof(CMD_STATION_TOPIC) - 1)

#define STATION_SETTINGS_ADDR_START 0x20

#define EEPROM_SETTINGS_VALID 0x00
#define EEPROM_SETTINGS_INVALID 0xFF

#define SETTINGS_SIZE 16

#define NUM_DEVICES 5

typedef struct connection_settings{
    char ssid[SETTINGS_SIZE];
    char password[SETTINGS_SIZE];
    char mqtt_server[SETTINGS_SIZE];
    uint16_t mqtt_port = 1883;
    char mqtt_id[SETTINGS_SIZE-2];
} connection_settings;

typedef byte device_state;
#define DEVICE_STATE_OFF 0
#define DEVICE_STATE_UNPOWERED 1
#define DEVICE_STATE_OVERRIDE 2
#define DEVICE_STATE_ON 3

#define CTRL_MASK 0x01
#define SENSE_MASK 0x02

static char state_str[][10] = {"OFF", "UNPOWERED", "OVERRIDE", "ON"};

typedef struct device_settings{
    char name[SETTINGS_SIZE-1] = "_";
    device_state state;   
} device_settings;


typedef struct station_settings{
    byte valid; // 0x00 if valid, 0xFF if not valid
    char fw_version[7] = "0.0.0";
    char name[SETTINGS_SIZE];
    byte padding[48-SETTINGS_SIZE-8];
    connection_settings conn;
    device_settings device[NUM_DEVICES];
} station_settings;

#endif // SETTINGS_H_
