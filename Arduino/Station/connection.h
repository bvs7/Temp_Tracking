#ifndef CONNECTION_H_
#define CONNECTION_H_

#define _ESPLOGLEVEL_ 0

#include "MQTT.h"
#include "WiFiEsp.h"
#include "settings.h"

bool publish(const char *topic, const char *payload, bool retain);
bool publish_data(const char *topic_suffix, const char *payload, bool retain);
bool subscribe(const char *topic, int qos);
bool unsubscribe(const char *topic);

void connection_setup( MQTTClientCallbackAdvanced callback);
void connection_loop();

extern MQTTClient mqtt_client;

#endif  // CONNECTION_H_
