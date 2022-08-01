/*
 * gemini-home automation Arduino MQTT project
*/

/*
 * LoopbackStream, feeds into serialcommands
 * Responses in both Serial and MQTT client
 */


#define _ESPLOGLEVEL_ 1
 
#include "utility.h"
#include "connection.h" // PubSubClient mqtt_client accessible
#include "commands.h"   // TODO Add other variables to commands

// From commands, command_handle is a stream that commands can be written to and results read from

#define P0_SENSE 5
#define P0_CTRL 4
#define P1_SENSE 7
#define P1_CTRL 6
#define P2_SENSE 9
#define P2_CTRL 8
#define P3_SENSE 11
#define P3_CTRL 10
#define P4_SENSE 13
#define P4_CTRL 12

sense_ctrl_port P0(P0_SENSE, P0_CTRL);
sense_ctrl_port P1(P1_SENSE, P1_CTRL);
sense_ctrl_port P2(P2_SENSE, P2_CTRL);
sense_ctrl_port P3(P3_SENSE, P3_CTRL);
sense_ctrl_port P4(P4_SENSE, P4_CTRL);

#define HEARTBEAT_PERIOD 5000
unsigned long heartbeat = 0;


// mqtt_callback
void callback(const char* topic, byte* payload, unsigned int length){
  // Check topic:
  // Station topic, submit to command buffer
  // Assume topic is correct
  for(int i=0; i<length; i++){
    command_handle.write(payload[i]);
  }
  command_handle.print("\n\0");
  // Device topic, handle device
}

void setup()
{
  util_setup();
  commands_setup();
  connection_setup();

  mqtt_client.setCallback(callback);

}

void loop()
{
  connection_loop();
  commands_loop();

  unsigned long current_millis = millis();
  if(current_millis > heartbeat){
    while(heartbeat < current_millis){
      heartbeat += HEARTBEAT_PERIOD;
    }
    dot();
  }
  
}
