#ifndef UTILITY_H_
#define UTILITY_H_

#include "Arduino.h"

#define CMD_STATION_TOPIC "cmd/irrigation/garden_station0"
#define CMD_STATION_TOPIC_LEN (sizeof(CMD_STATION_TOPIC) - 1)
#define DATA_STATION_TOPIC "data/irrigation/garden_station0"
#define DATA_STATION_TOPIC_LEN (sizeof(DATA_STATION_TOPIC) - 1)
#define ERROR_STATION_TOPIC "error/irrigation/garden_station0"
#define ERROR_STATION_TOPIC_LEN (sizeof(ERROR_STATION_TOPIC) - 1)

void dot();
void dash();
void space();
void util_setup();

#define SETTINGS_SIZE 32

#define CTRL_MASK 0x01
#define STATE_MASK 0x02
enum state{OFF_ = 0, UNPOWERED = 1, OVERRIDE = 2, ON_ = 3};

extern char state_string[4][10];

#endif
