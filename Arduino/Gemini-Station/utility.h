#ifndef UTILITY_H_
#define UTILITY_H_

#define LOG(x) Serial.print(x)


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


extern char state_string[4][10];

#endif
