#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Arduino.h"
#include "EEPROM.h"

#define CMD "cmd/"
#define DATA "data/"
#define LOG "log/"

#define SEC 1000

#define EEPROM_SETTINGS_VALID 0x00
#define EEPROM_SETTINGS_INVALID 0xFF


#define SETTING_LEN 16
#define NAME_LEN 8

#define NUM_P_DEVICES 5
#define NUM_A_DEVICES 0

#define UNUSED_PIN 0xFF
#define A_REQUEST_FLAG -1

// Device state
typedef byte device_state;

#define CTRL_MASK 0x01
#define SENSE_MASK 0x02
#define P_REQUEST_FLAG 0x04

static char state_str[][11] = 
    {"OFF+UNSET", "ON!UNSET", "OFF!SET", "ON+SET",
    "*OFF+UNSET", "*ON!UNSET", "*OFF!SET", "*ON+SET"};

typedef struct setting{
    char s[SETTING_LEN];
}setting;

// Connection settings addresses
#define VALID 0x000
#define FW_VERSION 0x001

#define STATION_NAME 0x010

#define SSID 0x020
#define PASSWD (SSID + SETTING_LEN)
#define MQTT_SERVER (PASSWD + SETTING_LEN)
#define MQTT_PORT (MQTT_SERVER + SETTING_LEN)

#define P 0x060

#define P_SENSE(x) (P + (x * 4))
#define P_CTRL(x) (P_SENSE(x) + 1)
#define P_STATE(x) (P_CTRL(x) + 1)

#define A 0x080

#define A_INPUT(x) (A + (x * 4))
#define A_TRIG(x) (A_INPUT(x) + 1)
#define A_STATE(x) (A_TRIG(x) + 1)

#define A_POLL_INTERVAL 0x0A0

#define GET_BYTE(x) EEPROM.read(x)
#define GET_INT(x) (EEPROM.read(x) << 8) | EEPROM.read(x + 1)

#define GET_STR(x, s) \
    for(uint8_t i = 0; i < SETTING_LEN; i++){ \
        s[i] = EEPROM.read(x + i); \
    }

#endif // SETTINGS_H_
