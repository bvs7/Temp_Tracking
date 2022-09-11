#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Arduino.h"

#define CMD "cmd"
#define DATA "data"
#define LOG "log"
#define IRRIGATION "irrigation"
#define TOPIC_INFIX "/irrigation/"

#define SEC 1000

#define EEPROM_SETTINGS_VALID 0x00
#define EEPROM_SETTINGS_INVALID 0xFF

#define FW_VERSION "0.0.0"

#define SETTING_LEN 16
#define NAME_LEN 8

#define NUM_P_DEVICES 5
#define NUM_A_DEVICES 4

#define UNUSED_PIN 0xFF
#define A_REQUEST_FLAG -1

typedef byte device_state;

#define CTRL_MASK 0x01
#define SENSE_MASK 0x02
#define P_REQUEST_FLAG 0x04

static char state_str[][11] = 
    {"OFF+UNSET", "ON!UNSET", "OFF!SET", "ON+SET",
    "*OFF+UNSET", "*ON!UNSET", "*OFF!SET", "*ON+SET"};

typedef struct connection_config{
    char ssid[SETTING_LEN];
    char password[SETTING_LEN];
    char mqtt_server[SETTING_LEN];
    char station_name[SETTING_LEN];
    uint16_t mqtt_port;
} connection_config;

typedef struct p_device_config{
    byte sense;
    byte ctrl;
    char name[NAME_LEN];
} p_device_config;

typedef struct a_device_config{
    byte input;
    byte trig;
    char name[NAME_LEN];
} a_device_config;

typedef struct station_meta{
    byte valid;
    char fw_version[15];
} station_meta;

typedef struct station_config{
    connection_config connection;
    p_device_config P[NUM_P_DEVICES];
    int a_device_poll_interval;
    a_device_config A[NUM_A_DEVICES];
} station_config;

typedef struct station_state{
    device_state P_state[NUM_P_DEVICES];
    int          A_value[NUM_A_DEVICES];
} station_state;

typedef struct station_settings{
    station_meta meta;
    station_config config;
    station_state state;
} station_settings;

boolean load_settings(station_settings *settings);

void save_config(station_config *config);

//#define INIT_SETTINGS_MODE

#ifdef SAVE_SETTINGS_MODE
void save_settings(station_settings *settings);
#endif // INIT_SETTINGS_MODE

#endif // SETTINGS_H_
