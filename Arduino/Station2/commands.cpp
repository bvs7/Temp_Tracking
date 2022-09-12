#include "Arduino.h"


#include "commands.h"

#include "utility.h"
#include "settings.h"
#include "connection.h"
#include "devices.h"


uint8_t pin_name_to_num(char *pin){
    if(pin[0] == 'A'){
        return atoi(pin+1) + 14;
    } else if(pin[0] == '-'){
        return UNUSED_PIN;
    } else {
        return atoi(pin);
    }
}

void pin_num_to_name(uint8_t pin, char *name){
    if(pin > 13){
        sprintf(name, "A%d", pin - 14);
    } else if(pin == UNUSED_PIN){
        sprintf(name, "-");
    } else {
        sprintf(name, "%d", pin);
    }
}

char config_names[][13] = 
    {"ssid", "password", "mqtt_server", "station_name", "mqtt_port"};

void root_handle(char *input, Stream *resp){
    char *saveptr;
    char *cmd = strtok_r(input, " ", &saveptr);
    if(cmd == NULL){
        // Empty? What to do?
        return;
    }
    if(strcmp(cmd, "hello") == 0){
        resp->println("Hello!");
        return;
    }else if(strcmp(cmd, "reboot") == 0){
        // TODO: Figure out watchdog timer
        resp->println("Can't reboot...");
        return;
    }
    for(int i = 0; i < 5; i++){
        if(strcmp(config_names[i], cmd) == 0){
            char *value = strtok_r(NULL, " ", &saveptr);
            if(value == NULL){ // Read config value
                resp->print(config_names[i]);
                resp->print(": ");
                char *result;
                switch(i){
                    case 0: result = get_str(SSID_, SETTING_LEN); break;
                    case 1: result = get_str(PASSWD, SETTING_LEN); break;
                    case 2: result = get_str(MQTT_SERVER, SETTING_LEN); break;
                    case 3: result = station_name; break;
                    case 4: {
                        char port[6];
                        itoa(get_int(MQTT_PORT), port, 10);
                        result = port;
                        break;
                    } 
                }
                resp->println(result);
                switch(i){
                    case 0:
                    case 1:
                    case 2:{
                        free(result);
                        break;
                    }
                    default:
                        break;
                }
                return;
            }
            // write config value
            int dest_addr;
            switch(i){
                case 0: dest_addr = SSID_; break;
                case 1: dest_addr = PASSWD; break;
                case 2: dest_addr = MQTT_SERVER; break;
                case 3: {
                    dest_addr = STATION_NAME; 
                    memset(station_name, 0, SETTING_LEN);
                    strncpy(station_name, value, SETTING_LEN-1);
                    break;
                }
                default: break;          
            }
            resp->print(config_names[i]);
            resp->print(" -> ");
            if(i == 4){
                set_int(MQTT_PORT, atoi(value));
                resp->println(get_int(MQTT_PORT));
            }else{
                set_str(dest_addr, value,SETTING_LEN);
                resp->println(value);
            }
            return;
        }
    }
    if(cmd[0] == 'P'){
        uint8_t id = cmd[1] - '0';
        if(id < 0 || id >= NUM_P_DEVICES){
            ERROR("Invalid P ID", "");
            return;
        }else{
            p_device_handle(id, cmd, &saveptr, resp);
        }
    }else if(cmd[0] == 'A'){
        uint8_t id = cmd[1] - '0';
        if(id < 0 || id >= NUM_A_DEVICES){
            ERROR("Invalid A ID", "");
            return;
        }else{
            a_device_handle(id, cmd, &saveptr, resp);
        }
    }
}

void p_device_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp){
    char *subcmd = strtok_r(NULL, " ", saveptr);
    if(subcmd == NULL){
        resp->print(cmd);
        resp->print(": sense=");
        resp->print(get_byte(P_SENSE(idx)));
        resp->print(", ctrl=");
        resp->print(get_byte(P_CTRL(idx)));
        resp->print(", state=");
        resp->println(state_str[p_states[idx]]);
        p_states[idx] |= P_REQUEST_FLAG;
        return;
    }
    char *value = strtok_r(NULL, " ", saveptr);
    resp->print(cmd);
    if(strcmp(subcmd, "state") == 0){
        resp->print(".state: ");
        resp->println(state_str[p_states[idx]]);
        p_states[idx] |= P_REQUEST_FLAG;
    }else if(strcmp(subcmd, "sense") == 0){
        if(value == NULL){
            resp->print(".sense: ");
            resp->println(get_byte(P_SENSE(idx)));
        }else{
            set_byte(P_SENSE(idx), pin_name_to_num(value));
            resp->print(".sense -> ");
            resp->println(get_byte(P_SENSE(idx)));
            devices_setup();
        }
    }else if(strcmp(subcmd, "ctrl") == 0){
        if(value == NULL){
            resp->print(".ctrl: ");
            resp->println(get_byte(P_CTRL(idx)));
        }else{
            set_byte(P_CTRL(idx), pin_name_to_num(value));
            resp->print(".ctrl -> ");
            resp->println(get_byte(P_CTRL(idx)));
            devices_setup();
        }
    }else if(strcmp(subcmd, "ON") == 0){
        p_states[idx] |= CTRL_MASK;
        p_states[idx] |= P_REQUEST_FLAG;
        resp->println(" -> ON");
    }else if(strcmp(subcmd, "OFF") == 0){
        p_states[idx] &= ~CTRL_MASK;
        p_states[idx] |= P_REQUEST_FLAG;
        resp->println(" -> OFF");
    }else{
        resp->println(" ???");
    }
}

void a_device_config_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp){
    char *subcmd = strtok_r(NULL, " ", saveptr);
    if(subcmd == NULL){
        resp->print(cmd);
        resp->print(": input=");
        resp->print(get_byte(A_INPUT(idx)));
        resp->print(", trig=");
        resp->print(get_byte(A_TRIG(idx)));
        resp->print(", value=");
        resp->println(a_values[idx]);
        a_values[idx] = A_REQUEST_FLAG;
        return;
    }
    resp->print(cmd);
    char *value = strtok_r(NULL, " ", saveptr);
    if(strcmp(subcmd, "value") == 0){
        resp->print(".value: ");
        resp->println(a_values[idx]);
        a_values[idx] = A_REQUEST_FLAG;
    }else if(strcmp(subcmd, "input") == 0){
        char *value = strtok(NULL, " ");
        if(value == NULL){
            resp->print(".input: ");
            char name[4];
            pin_num_to_name(get_byte(A_INPUT(idx)), name);
            resp->println(name);
        }else{
            set_byte(A_INPUT(idx), pin_name_to_num(value));
            resp->print(".input -> ");
            char name[4];
            pin_num_to_name(get_byte(A_INPUT(idx)), name);
            resp->println(name);
            devices_setup();
        }
    }else if(strcmp(subcmd, "trig") == 0){
        if(value == NULL){
            resp->print(".trig: ");
            char name[4];
            pin_num_to_name(get_byte(A_TRIG(idx)), name);
            resp->println(name);
        }else{
            set_byte(A_TRIG(idx), pin_name_to_num(value));
            resp->print(".trig -> ");
            char name[4];
            pin_num_to_name(get_byte(A_TRIG(idx)), name);
            resp->println(name);
            devices_setup();
        }
    }else if(strcmp(subcmd, "UPDATE") == 0){
        a_values[idx] = A_REQUEST_FLAG;
        resp->println(" -> UPDATE");
    }else{
        resp->println(" ???");
    }
}
