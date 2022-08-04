#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Arduino.h"

#include "WiFiEsp.h" // WifiEsp library for wifi with esp8266
#include "PubSubClient.h" // PubSubClient library for MQTT

#include "utility.h"

#define SETTINGS_SIZE 32

#define MQTT_PORT 1883

typedef struct connection_settings{
  char ssid[SETTINGS_SIZE];
  char pass[SETTINGS_SIZE];
  char server[SETTINGS_SIZE];
  char mqtt_id[SETTINGS_SIZE];
} connection_settings_t;


// Define a class connection with arrays for ssid, pass, server, and mqtt_id.
// It has a wifi_client and mqtt_client instance.
// It has a setup and loop function
class connection{
  
  public:
  connection_settings_t settings;
  WiFiEspClient wifi_client;
  PubSubClient mqtt_client;
  
  connection(const char* station_name);
  
  void save();
  void load();
  
  void setup();
  void loop();
  
};

//bool wifi_connect();
//bool mqtt_connect();
// Use mqtt_client.setCallback() to set callback
//bool reconnect();
//void save_conn();

void connection_setup();
void connection_loop();

char * connection_get_setting(const char * setting);
bool connection_set_setting(const char * setting, const char * value);

extern PubSubClient mqtt_client;

#endif
