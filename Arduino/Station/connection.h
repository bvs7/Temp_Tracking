#ifndef CONNECTION_H_
#define CONNECTION_H_

#define _ESPLOGLEVEL_ 0

#include "PubSubClient.h"
#include "WiFiEsp.h"
#include "settings.h"

bool publish(const char *topic, const char *payload, bool retain);
bool subscribe(const char *topic, int qos);
bool unsubscribe(const char *topic);

void connection_setup(MQTT_CALLBACK_SIGNATURE);
void connection_loop();


#endif  // CONNECTION_H_
