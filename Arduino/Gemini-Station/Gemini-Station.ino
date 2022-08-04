/*
 * gemini-home automation Arduino MQTT project
*/

#define _ESPLOGLEVEL_ 0

#include "utility.h"
#include "devices.h"
#include "connection.h" // PubSubClient mqtt_client accessible
#include "commands.h" // SerialCommand cmd_set_ cmd_get_ cmd_echo_

// From commands, command_handle is a stream that commands can be written to and results read from

#define P0_SENSE 5
#define P0_CTRL 13
#define P1_SENSE 7
#define P1_CTRL 6
#define P2_SENSE 9
#define P2_CTRL 8
#define P3_SENSE 11
#define P3_CTRL 10
#define P4_SENSE 13 // warning, LED_BUILTIN
#define P4_CTRL 12

device P0(P0_SENSE, P0_CTRL, "/pump0");
device P1(P1_SENSE, P1_CTRL, "/valve0_trees");

#define HEARTBEAT_PERIOD 1000
unsigned long heartbeat = 0;

int parse_device_value(const char* value){
  if(strcmp(value, "ON") == 0){
    return 1;
  }else if(strcmp(value, "OFF") == 0){
    return 0;
  }else{
    return -1;
  }
}

char * get_setting(const char *var){
  char *value = NULL;
  value = connection_get_setting(var);
  if(value != NULL){
    return value;
  }
  if(strcmp(var, P0.device_name) == 0){
    return P0.state_str();
  }
  if(strcmp(var, P1.device_name) == 0){
    return P1.state_str();
  }
  return NULL;
}

bool set_setting(const char *var, const char *value){
  int v = parse_device_value(value);
  if(strcmp(var, P0.device_name) == 0){
    if(v != -1){
      P0.set_ctrl(v);
    }else{
    }
    return true;
  }
  if(strcmp(var, P1.device_name) == 0){
    if(v != -1){
      P1.set_ctrl(v);
    }else{
    }
    return true;
  }
  if(connection_set_setting(var, value)){
    return true;
  }
  return false;
}

void mqtt_on_connect(){
  mqtt_client.subscribe("debug/test");
  mqtt_client.publish("debug/test", "Connected");
}

void mqtt_callback(char* topic, byte* payload, unsigned int len) {
  // compare to CMD_STATION_TOPIC
  if(strncmp(topic, CMD_STATION_TOPIC, CMD_STATION_TOPIC_LEN) == 0){
    Serial.println("Got Command");
    // Check if topic is only CMD_STATION_TOPIC
    if(strlen(topic) == CMD_STATION_TOPIC_LEN){
      Serial.println("Station command");
      enter_command((char*)payload, len);
    }else{
      Serial.println("Other command");
      char* var = topic + CMD_STATION_TOPIC_LEN;
      unsigned int var_len = len - CMD_STATION_TOPIC_LEN;
      Serial.print(var);
      Serial.println(" var");
      Serial.println((char*)payload);
      if(!set_setting(var, (char*)payload)){
        Serial.print("Error: invalid setting ");
        Serial.print(var);
        Serial.print("=");
        Serial.println((char*)payload);
      }
    }
  }
}


void setup(){
  util_setup();
  commands_setup();
  connection_setup();

  mqtt_client.setCallback(mqtt_callback);

  P0.setup();
  P1.setup();

}

void loop(){
  commands_loop();

  unsigned long current_millis = millis();
  if(current_millis > heartbeat){
    while(heartbeat < current_millis){
      heartbeat += HEARTBEAT_PERIOD;
    }
    P0.loop();
    P1.loop();
    dot();
  }
  
}
