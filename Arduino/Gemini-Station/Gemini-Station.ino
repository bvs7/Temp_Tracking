/*
 * gemini-home automation Arduino MQTT project
*/
#include "utility.h"
#include "connection.h"
#include "commands.h"

void mqtt_on_connect(){
  mqtt_client.subscribe("debug/test");
  mqtt_client.publish("debug/test", "Connected");
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Recieved message on topic ");
  Serial.print(topic);
  Serial.println(":");
  Serial.println(payload);
  dot();
}

void setup(){
  util_setup();
  commands_setup();
  connection_setup(mqtt_on_connect);

  mqtt_client.setCallback(mqtt_callback);
}

void loop(){
  commands_loop();
  connection_loop();
  // Poll sense pins
}
