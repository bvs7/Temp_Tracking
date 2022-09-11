#ifndef DEVICES_H_
#define DEVICES_H_

#include "settings.h"

int dist_sensor(int trig_pin, int echo_pin);
void update_a_device(uint8_t idx);
uint8_t p_id_from_name(char *name);
uint8_t a_id_from_name(char *name);
void p_device_setup(p_device_config *p, device_state s);
void a_device_setup(a_device_config *a, int *value);

void devices_setup(station_settings *s);
void devices_loop();

#endif // DEVICES_H_

