
#include "commands.h"

#include "Arduino.h"
#include "connection.h"
#include "devices.h"
#include "settings.h"
#include "utility.h"

#define FILE_ "comm: "

void homie_mqtt_handle(char *topic, byte *payload, unsigned int length){
    WARN("mqtt msg ", topic);
    if(strncmp(topic, category, strlen(category)) != 0){
        ERR(FILE_, __LINE__);  // ERROR("Invalid topic");
        return;
    }
    if(strncmp(topic+strlen(category)+1, station_name, strlen(station_name)) != 0){
        ERR(FILE_, __LINE__);  // ERROR("Invalid station name");
        return;
    }
    size_t node_offset = strlen(category) + strlen(station_name) + 2;
    uint8_t node_idx = topic[node_offset+1] - '0';
    if(node_idx >= NUM_P_DEVICES){
        ERR(FILE_, __LINE__);  // ERROR("Invalid node index");
        return;
    }

    if(strncmp(topic+node_offset+3, "ctrl", 4) != 0){
        ERR(FILE_, __LINE__);  // ERROR("Invalid node command");
        return;
    }
    
    if(strncmp((char *)payload, "true", 4) == 0){
        p_ctrl_set(node_idx, true);
    }else if(strncmp((char *)payload, "false", 5) == 0){
        p_ctrl_set(node_idx, false);
    }else{
        ERR(FILE_, __LINE__);  // ERROR("Invalid payload");
        return;
    }

}
