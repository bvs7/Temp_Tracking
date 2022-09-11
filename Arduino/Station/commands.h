#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "settings.h"

void root_handle(char *input, Stream *resp);

void p_device_config_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp);
void a_device_config_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp);

void p_device_state_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp);
void a_device_state_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp);

void commands_setup(station_settings *s);

// void *p_device_read_setting(p_device *device, char *key);
// bool p_device_write_setting(p_device *device, char *key, char *value);
// void *a_device_read_setting(a_device *device, char *key);
// bool a_device_write_setting(a_device *device, char *key, char *value);

// bool parse_device_request(char *input);

extern station_settings station;

#endif // DEVICE_H_