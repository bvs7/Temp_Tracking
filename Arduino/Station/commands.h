#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "Arduino.h"

void mqtt_handle(char *topic, byte *payload, unsigned int length, Stream *resp);
void root_handle(char *input, Stream *resp);
bool settings_handle(char *sett, char **saveptr, Stream *resp);
void p_device_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp);
void a_device_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp);

#endif  // DEVICE_H_