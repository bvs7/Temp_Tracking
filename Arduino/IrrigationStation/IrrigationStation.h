#ifndef IRRIGATION_H_
#define IRRIGATION_H_

#include "connection.h"
#include "device.h"
#include "settings.h"
#include "utility.h"

void mqtt_callback(char* topic, char* message);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
bool handle_command(char* input, char** response, bool mqtt);
void handle_serial();
void save_settings();
void load_settings();

#endif
