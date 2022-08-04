#ifndef CONNECTION_H_
#define CONNECTION_H_

#define _ESPLOGLEVEL_ 0

#include "PubSubClient.h"
#include "WiFiEsp.h"
#include "settings.h"

bool publish_data(const char *topic_suffix, const char *message, bool retain);
bool publish_error(const char *message, bool retain);
bool subscribe(const char *topic, int qos);
bool unsubscribe(const char *topic);
PubSubClient *setCallback(MQTT_CALLBACK_SIGNATURE);

void connection_setup(station_settings *station);
void connection_loop(station_settings *station);

#endif // CONNECTION_H_