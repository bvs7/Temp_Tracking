#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Arduino.h"
#include "EEPROM.h"

#define CMD "cmd"
#define DATA "data"
#define LOG "log"

#define SEC 1000

#define EEPROM_SETTINGS_VALID 0x00
#define EEPROM_SETTINGS_INVALID 0xFF

#define SETTING_LEN 16

#define NUM_P_DEVICES 5
#define NUM_A_DEVICES 0

#define UNUSED_PIN 0xFF

// Connection settings addresses
#define VALID 0x000
#define FW_VERSION 0x001

#define STATION_NAME 0x010

#define WIFI_SSID (STATION_NAME + SETTING_LEN)
#define WIFI_PASSWD (WIFI_SSID + SETTING_LEN)
#define MQTT_SERVER (WIFI_PASSWD + SETTING_LEN)
#define MQTT_PORT (MQTT_SERVER + SETTING_LEN)

#define P 0x060

#define P_SENSE(x) (P + (x * 2))
#define P_CTRL(x) (P_SENSE(x) + 1)

#define A 0x070

#define A_INPUT(x) (A + (x * 2))
#define A_TRIG(x) (A_INPUT(x) + 1)

#define A_POLL_INTERVAL 0x080

// Device state
typedef byte p_state;
#define CTRL_MASK 0x01
#define SENSE_MASK 0x02
#define P_REQUEST_FLAG 0x04

extern char state_str[][11];

typedef int a_value;
#define A_REQUEST_FLAG -1

extern p_state p_states[];
extern a_value a_values[];

extern char *station_name;

byte get_byte(int addr);
int get_int(int addr);
char *get_str(int addr, size_t length);

void set_byte(int addr, byte val);
void set_int(int addr, int val);
void set_str(int addr, char *val, size_t length);

#endif  // SETTINGS_H_
