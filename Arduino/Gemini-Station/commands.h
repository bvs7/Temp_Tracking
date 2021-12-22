#ifndef COMMANDS_H_
#define COMMANDS_H_

#define DEBUG_BAUD_RATE 9600

#define SETTINGS_SIZE 32

typedef struct connection_settings{
  char ssid[SETTINGS_SIZE];
  char pass[SETTINGS_SIZE];
  char server[SETTINGS_SIZE];
  char mqtt_id[SETTINGS_SIZE];
} connection_settings_t;


void commands_setup();
void commands_loop();

extern connection_settings_t conn_sett;

#endif
