#ifndef DEVICES_H_
#define DEVICES_H_

#include "Arduino.h"

int dist_sensor(int trig_pin, int echo_pin);
void p_ctrl_set(uint8_t idx, bool set);
void update_p_device(uint8_t idx);
void p_device_setup(uint8_t idx);

void homie_setup(char *category, char *station_name);
void homie_pnode_setup(char *n_id, char *topic, char *sett, char *n_type);
void publish_homie_sett(char *base_topic, char *sett_loc,
                        const char *sett_name, const char *sett_value);

void devices_setup();
void devices_loop();

extern unsigned long seconds;

#endif  // DEVICES_H_
