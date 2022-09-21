#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "Arduino.h"

void homie_mqtt_handle(char *topic, byte *payload, unsigned int length);

#endif  // DEVICE_H_