#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Arduino.h"

#include "WiFiEsp.h"
#include "PubSubClient.h"
#include "commands.h"

void connection_setup();
void connection_loop();

bool wifi_connect();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void mqtt_onConnect();
bool mqtt_connect();
bool reconnect();

extern WiFiEspClient wifi_client;
extern PubSubClient mqtt_client;

#endif
