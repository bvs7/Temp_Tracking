#ifndef CONNECTION_H_
#define CONNECTION_H_

#define _ESPLOGLEVEL_ 0

#include "PubSubClient.h"
#include "WiFiEsp.h"
#include "settings.h"

bool publish_data(const char *topic_suffix, const char *message, bool retain);
bool publish_log(const char *message, bool retain);
bool subscribe(const char *topic, int qos);
bool unsubscribe(const char *topic);
PubSubClient *set_callback(MQTT_CALLBACK_SIGNATURE);

void connection_setup(connection_config *conn);
void connection_loop();

#endif // CONNECTION_H_
