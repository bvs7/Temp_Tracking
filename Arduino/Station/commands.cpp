#include "Arduino.h"

#include "utility.h"
#include "settings.h"
#include "commands.h"
#include "connection.h"

#include <Adafruit_SleepyDog.h>

station_settings *sett;

void (*reset) (void) = 0;

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
    resp->print("#");
    char *saveptr;
    char *cmd = strtok_r(input, " ", &saveptr);
    Serial.println(cmd);
    if(cmd == NULL){
        // Empty? What to do?
        return;
    }
    if(strcmp(cmd, "hello") == 0){
        resp->println("Hello!");
        return;
    }else if(strcmp(cmd, "reboot") == 0){
        // TODO: Figure out watchdog timer
        reset();
        delay(5000);
        resp->println("Rebooting...");
        Watchdog.enable(2000);
        return;
    }
    for(int i = 0; i < 5; i++){
        if(strcmp(config_names[i], cmd) == 0){
            char *value = strtok_r(NULL, " ", &saveptr);
            if(value == NULL){ // Read config value
                resp->print(config_names[i]);
                resp->print(": ");
                switch(i){
                  case 0: resp->println(sett->config.connection.ssid); break;
                  case 1: resp->println(sett->config.connection.password); break;
                  case 2: resp->println(sett->config.connection.mqtt_server); break;
                  case 3: resp->println(sett->config.connection.station_name); break;
                  case 4: resp->println(sett->config.connection.mqtt_port); break;
                }
                return;
            }
            // write config value
            char *dest;
            switch(i){
            case 0: dest = sett->config.connection.ssid; break;
            case 1: dest = sett->config.connection.password; break;
            case 2: dest = sett->config.connection.mqtt_server; break;
            case 3: dest = sett->config.connection.station_name; break;
            }
            resp->print(config_names[i]);
            resp->print(" -> ");
            if(i == 4){
                sett->config.connection.mqtt_port = atoi(value);
                resp->println(sett->config.connection.mqtt_port);
            }else{
                strncpy(dest, value, SETTING_LEN-1);
                resp->println(dest);
            }
            save_config(&sett->config);
            return;
        }
    }
    if(cmd[0] == 'P'){
        int id = cmd[1] - '0';
        if(id < 0 || id >= NUM_P_DEVICES){
            // Error
            resp->println("Invalid device ID");
            return;
        }else{
            p_device_config_handle(id, cmd, &saveptr, resp);
        }
    }else if(cmd[0] == 'A'){
        int id = cmd[1] - '0';
        if(id < 0 || id >= NUM_A_DEVICES){
            // Error
            resp->println("Invalid device ID");
            return;
        }else{
            a_device_config_handle(id, cmd, &saveptr, resp);
        }
    }
}

void p_device_config_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp){
    p_device_config *P = &sett->config.P[idx];
    char *subcmd = strtok_r(NULL, " ", saveptr);
    if(subcmd == NULL){
        resp->print(cmd);
        resp->print(": sense=");
        resp->print(P->sense);
        resp->print(", ctrl=");
        resp->print(P->ctrl);
        resp->print(", name=");
        resp->println(P->name);
        return;
    }
    char *value = strtok_r(NULL, " ", saveptr);
    if(strcmp(subcmd, "name") == 0){
        if(value == NULL){
            resp->print(cmd);
            resp->print(".name: ");
            resp->println(P->name);
            return;
        }
        strncpy(P->name, value, NAME_LEN-1);
        resp->print(cmd);
        resp->print(".name -> ");
        resp->println(P->name);
    }else if(strcmp(subcmd, "sense") == 0){
        if(value == NULL){
            resp->print(cmd);
            resp->print(".sense: ");
            char name[4];
            pin_num_to_name(P->sense, name);
            resp->println(name);
            return;
        }
        P->sense = pin_name_to_num(value);
        resp->print(cmd);
        resp->print(".sense -> ");
        char name[4];
        pin_num_to_name(P->sense, name);
        resp->println(name);
    }else if(strcmp(subcmd, "ctrl") == 0){
        if(value == NULL){
            resp->print(cmd);
            resp->print(".ctrl: ");
            char name[4];
            pin_num_to_name(P->ctrl, name);
            resp->println(name);
            return;
        }
        P->ctrl = pin_name_to_num(value);
        resp->print(cmd);
        resp->print(".ctrl -> ");
        char name[4];
        pin_num_to_name(P->ctrl, name);
        resp->println(name);
    }else{
        // invalid subcommand
    }
    save_config(&sett->config);
}

void a_device_config_handle(uint8_t id, char *cmd, char **saveptr, Stream *resp){
    a_device_config *A = &sett->config.A[id];
    char *subcmd = strtok_r(NULL, " ", saveptr);
    if(subcmd == NULL){
        resp->print(cmd);
        resp->print(": input=");
        resp->print(A->input);
        resp->print(", trig=");
        resp->print(A->trig);
        resp->print(", name=");
        resp->println(A->name);
        return;
    }
    char *value = strtok_r(NULL, " ", saveptr);
    if(strcmp(subcmd, "name") == 0){
        if(value == NULL){
            resp->print(cmd);
            resp->print(".name: ");
            resp->println(A->name);
            return;
        }
        strncpy(A->name, value, NAME_LEN-1);
        resp->print(cmd);
        resp->print(".name -> ");
        resp->println(A->name);
    }else if(strcmp(subcmd, "input") == 0){
        char *value = strtok(NULL, " ");
        if(value == NULL){
            resp->print(cmd);
            resp->print(".input: ");
            char name[4];
            pin_num_to_name(A->input, name);
            resp->println(name);
            return;
        }
        A->input = pin_name_to_num(value);
        resp->print(cmd);
        resp->print(".input -> ");
        char name[4];
        pin_num_to_name(A->input, name);
        resp->println(name);
    }else if(strcmp(subcmd, "trig") == 0){
        char *value = strtok(NULL, " ");
        if(value == NULL){
            resp->print(cmd);
            resp->print(".trig: ");
            char name[4];
            pin_num_to_name(A->trig, name);
            resp->println(name);
            return;
        }
        A->trig = pin_name_to_num(value);
        resp->print(cmd);
        resp->print(".trig -> ");
        char name[4];
        pin_num_to_name(A->trig, name);
        resp->println(name);
    }else{
        // invalid subcommand
        return;
    }
    save_config(&sett->config);
}

void p_device_state_handle(uint8_t id, char *cmd, char **saveptr, Stream *resp){
    device_state *state = &sett->state.P_state[id];
    char *subcmd = strtok_r(NULL, " ", saveptr);
    if(subcmd == NULL){
        // Prompt data publish
        *state |= P_REQUEST_FLAG;
        resp->print(cmd);
        resp->print(": ");
        resp->print(state_str[*state]);
    }else{
        bool set = false;
        set = set || (strcmp(subcmd, "ON") == 0);
        digitalWrite(sett->config.P[id].ctrl, set);
        resp->print(cmd);
        resp->print(" -> ");
        resp->println(set ? "ON" : "OFF");
    }
}

void a_device_state_handle(uint8_t id, char *cmd, char **saveptr, Stream *resp){
    int *value = &sett->state.A_value[id];
    resp->print(cmd);
    resp->print(": ");
    resp->println(*value);
    *value = A_REQUEST_FLAG;
}


void commands_setup(station_settings *s){
    sett = s;
}
