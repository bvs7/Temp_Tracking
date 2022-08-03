/*
 * gemini-home automation Arduino MQTT project
*/

#define DEBUG_

#ifdef DEBUG_
#define _ESPLOGLEVEL_ 4
#else
#define _ESPLOGLEVEL_ 0
#endif

#include "utility.h"
#include "devices.h"
#include "connection.h" // PubSubClient mqtt_client accessible
#include "commands.h" // SerialCommand cmd_set_ cmd_get_ cmd_echo_

// From commands, command_handle is a stream that commands can be written to and results read from

#define P0_SENSE 5
#define P0_CTRL 4
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

#define HEARTBEAT_PERIOD 5000
unsigned long heartbeat = 0;

void station_command(char * payload, unsigned int length){
  // Handle station endpoint cmd/irrigation/garden_station0
  for(int i=0; i<length; i++){
    command_handle.write(payload[i]);
  }
  command_handle.print("\n\0");
}

bool device_command(device* P, char * topic_suffix, unsigned int suffix_len, 
                               char * payload, unsigned int length){
  if(P->check_name(topic_suffix, suffix_len)){    
    if(strncmp(payload, state_string[ON_], length)){
      P->set_ctrl(HIGH);
    } else if(strncmp(payload, state_string[OFF_], length)){
      P->set_ctrl(LOW);
    } else {
      mqtt_client.publish(
        strcat(ERROR_STATION_TOPIC, P0.device_name), 
        strncat("Invalid state (not \"ON\"/\"OFF\"): ", payload, length)
      );
      command_internal.println(strcat(ERROR_STATION_TOPIC, P0.device_name));
      command_internal.println(
        strncat("Invalid state (not \"ON\"/\"OFF\"): ", payload, length));
    }
    return true;
  }
  return false;
}

void mqtt_on_connect(){
  mqtt_client.subscribe("debug/test");
  mqtt_client.publish("debug/test", "Connected");
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // compare to CMD_STATION_TOPIC
  if(strncmp(topic, CMD_STATION_TOPIC, CMD_STATION_TOPIC_LEN) == 0){
    // Check if topic is only CMD_STATION_TOPIC
    if(strlen(topic) == CMD_STATION_TOPIC_LEN){
      station_command((char*)payload, length);
    }else{
      char* topic_suffix = topic + CMD_STATION_TOPIC_LEN;
      unsigned int suffix_len = length - CMD_STATION_TOPIC_LEN;
      if(device_command(&P0, topic_suffix, suffix_len, (char*)payload, length)){
        return;
      }
      if(device_command(&P1, topic_suffix, suffix_len, (char*)payload, length)){
        return;
      }
    }
  }
}

void setup(){
  util_setup();
  commands_setup();
  connection_setup();

  mqtt_client.setCallback(mqtt_callback);

}

void loop(){
  commands_loop();

  unsigned long current_millis = millis();
  if(current_millis > heartbeat){
    while(heartbeat < current_millis){
      heartbeat += HEARTBEAT_PERIOD;
    }
    dot();
  }
  
}
