#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Arduino.h"

#include "WiFiEsp.h"
#include "PubSubClient.h"

#define SETTINGS_SIZE 32

#define MQTT_PORT 1883

typedef struct connection_settings{
  char ssid[SETTINGS_SIZE];
  char pass[SETTINGS_SIZE];
  char server[SETTINGS_SIZE];
  char mqtt_id[SETTINGS_SIZE];
} connection_settings_t;


bool wifi_connect();
bool mqtt_connect();
// Use mqtt_client.setCallback() to set callback
bool reconnect();
void save_conn();

void connection_setup(void (*mqtt_on_connect)(void));
void connection_loop();

extern WiFiEspClient wifi_client;
extern PubSubClient mqtt_client;
extern connection_settings_t conn_sett;

#endif
