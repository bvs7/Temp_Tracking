
#include "commands.h"

#include "Arduino.h"
#include "connection.h"
#include "devices.h"
#include "settings.h"
#include "utility.h"

uint8_t pin_name_to_num(char *pin) {
    if (pin[0] == 'A') {
        return atoi(pin + 1) + 14;
    } else if (pin[0] == '-') {
        return UNUSED_PIN;
    } else {
        return atoi(pin);
    }
}

void pin_num_to_name(uint8_t pin, char *name) {
    if (pin > 13) {
        sprintf(name, "A%d", pin - 14);
    } else if (pin == UNUSED_PIN) {
        sprintf(name, "-");
    } else {
        sprintf(name, "%d", pin);
    }
}

void mqtt_handle(char *topic, byte *payload, unsigned int length,
                 Stream *resp) {
    char *saveptr = NULL;
    char *mode = strtok_r(topic, "/", &saveptr);
    if (mode == NULL || strcmp(mode, CMD) != 0) {
        ERROR("Invalid topic: ", mode == NULL ? "NULL" : mode);
        return;
    }
    char *name = strtok_r(NULL, "/", &saveptr);
    if (name == NULL || strcmp(name, station_name) != 0) {
        ERROR("Invalid name: ", name == NULL ? "NULL" : name);
        return;
    }
    payload[length] = '\0';
    char *device_name = strtok_r(NULL, "/", &saveptr);
    if (device_name == NULL) {
        // Station command
        root_handle((char *)payload, resp);
    } else {
        // Device command
        if (device_name[0] == 'P') {
            uint8_t p_id = device_name[1] - '0';
            if (p_id >= NUM_P_DEVICES) {
                ERROR("Invalid P device #: ", device_name);
                return;
            }
            p_device_handle(p_id, device_name, (char **)&payload, resp);
        } else if (device_name[0] == 'A') {
            uint8_t a_id = device_name[1] - '0';
            if (a_id >= NUM_A_DEVICES) {
                ERROR("Invalid A device #: ", device_name);
                return;
            }
            a_device_handle(a_id, device_name, (char **)&payload, resp);
        } else {
            ERROR("Invalid device name: ", device_name);
            return;
        }
    }
}

void root_handle(char *input, Stream *resp) {
    char *saveptr = NULL;
    char *cmd = strtok_r(input, " ", &saveptr);
    if (cmd == NULL) {
        // Empty? What to do?
        return;
    }
    if (strcmp(cmd, "hello") == 0) {
        resp->println("Hello!");
        return;
    } else if (strcmp(cmd, "reboot") == 0) {
        // TODO: Figure out watchdog timer
        resp->println("Can't reboot...");
        return;
    } else if (strcmp(cmd, "mqtt") == 0) {
        char *topic = strtok_r(NULL, " ", &saveptr);
        if (topic == NULL) {
            ERROR("No topic specified", "");
            return;
        }
        byte *payload = (byte *)strtok_r(NULL, "\0", &saveptr);
        unsigned int length = strlen((char *)payload);
        mqtt_handle(topic, payload, length, resp);
    } else if (connection_handle(cmd, &saveptr, resp)) {
        return;
    } else if (cmd[0] == 'P') {
        uint8_t id = cmd[1] - '0';
        if (id < 0 || id >= NUM_P_DEVICES) {
            ERROR("Invalid P ID", "");
            return;
        } else {
            p_device_handle(id, cmd, &saveptr, resp);
        }
    } else if (cmd[0] == 'A') {
        uint8_t id = cmd[1] - '0';
        if (id < 0 || id >= NUM_A_DEVICES) {
            ERROR("Invalid A ID", "");
            return;
        } else {
            a_device_handle(id, cmd, &saveptr, resp);
        }
    } else {
        ERROR("Invalid command", cmd);
    }
}

bool connection_handle(char *cmd, char **saveptr, Stream *resp) {
    char config_names[][13] = {"station_name", "ssid", "password",
                               "mqtt_server"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(config_names[i], cmd) == 0) {
            char *value = strtok_r(NULL, " ", saveptr);
            if (value == NULL) {  // Read config value
                resp->print(config_names[i]);
                resp->print(": ");
                char *result =
                    get_str(STATION_NAME + i * SETTING_LEN, SETTING_LEN);
                resp->println(result);
                free(result);
                return true;
            } else {  // Write config value
                resp->print(config_names[i]);
                resp->print(" -> ");
                set_str(STATION_NAME + i * SETTING_LEN, value, SETTING_LEN);
                resp->println(value);
                return true;
            }
        }
    }
    if (strcmp("mqtt_port", cmd) == 0) {
        char *value = strtok_r(NULL, " ", saveptr);
        if (value == NULL) {  // Read config value
            resp->print("mqtt_port");
            resp->print(": ");
            resp->println(get_int(MQTT_PORT));
            return true;
        } else {  // Write config value
            resp->print("mqtt_port");
            resp->print(" -> ");
            set_int(MQTT_PORT, atoi(value));
            resp->println(get_int(MQTT_PORT));
            return true;
        }
    }
    return false;
}

void p_device_handle(uint8_t idx, char *device, char **saveptr, Stream *resp) {
    char *subcmd = strtok_r(NULL, " ", saveptr);
    if (subcmd == NULL) {
        resp->print(device);
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
    resp->print(device);
    if (strcmp(subcmd, "state") == 0) {
        resp->print(".state: ");
        resp->println(state_str[p_states[idx]]);
        p_states[idx] |= P_REQUEST_FLAG;
    } else if (strcmp(subcmd, "sense") == 0) {
        if (value == NULL) {
            resp->print(".sense: ");
            resp->println(get_byte(P_SENSE(idx)));
        } else {
            set_byte(P_SENSE(idx), pin_name_to_num(value));
            resp->print(".sense -> ");
            resp->println(get_byte(P_SENSE(idx)));
            devices_setup();
        }
    } else if (strcmp(subcmd, "ctrl") == 0) {
        if (value == NULL) {
            resp->print(".ctrl: ");
            resp->println(get_byte(P_CTRL(idx)));
        } else {
            digitalWrite(get_byte(P_CTRL(idx)), LOW);
            set_byte(P_CTRL(idx), pin_name_to_num(value));
            resp->print(".ctrl -> ");
            resp->println(get_byte(P_CTRL(idx)));
            devices_setup();
        }
    } else if (strcmp(subcmd, "ON") == 0) {
        p_states[idx] |= CTRL_MASK;
        p_states[idx] |= P_REQUEST_FLAG;
        p_device_set(idx, HIGH);
        resp->println(" -> ON");
    } else if (strcmp(subcmd, "OFF") == 0) {
        p_states[idx] &= ~CTRL_MASK;
        p_states[idx] |= P_REQUEST_FLAG;
        p_device_set(idx, LOW);
        resp->println(" -> OFF");
    } else {
        resp->println(" ???");
    }
}

void a_device_config_handle(uint8_t idx, char *device, char **saveptr,
                            Stream *resp) {
    char *subcmd = strtok_r(NULL, " ", saveptr);
    if (subcmd == NULL) {
        resp->print(device);
        resp->print(": input=");
        resp->print(get_byte(A_INPUT(idx)));
        resp->print(", trig=");
        resp->print(get_byte(A_TRIG(idx)));
        resp->print(", value=");
        resp->println(a_values[idx]);
        a_values[idx] = A_REQUEST_FLAG;
        return;
    }
    resp->print(device);
    char *value = strtok_r(NULL, " ", saveptr);
    if (strcmp(subcmd, "value") == 0) {
        resp->print(".value: ");
        resp->println(a_values[idx]);
        a_values[idx] = A_REQUEST_FLAG;
    } else if (strcmp(subcmd, "input") == 0) {
        char *value = strtok(NULL, " ");
        if (value == NULL) {
            resp->print(".input: ");
            char name[4];
            pin_num_to_name(get_byte(A_INPUT(idx)), name);
            resp->println(name);
        } else {
            set_byte(A_INPUT(idx), pin_name_to_num(value));
            resp->print(".input -> ");
            char name[4];
            pin_num_to_name(get_byte(A_INPUT(idx)), name);
            resp->println(name);
            devices_setup();
        }
    } else if (strcmp(subcmd, "trig") == 0) {
        if (value == NULL) {
            resp->print(".trig: ");
            char name[4];
            pin_num_to_name(get_byte(A_TRIG(idx)), name);
            resp->println(name);
        } else {
            set_byte(A_TRIG(idx), pin_name_to_num(value));
            resp->print(".trig -> ");
            char name[4];
            pin_num_to_name(get_byte(A_TRIG(idx)), name);
            resp->println(name);
            devices_setup();
        }
    } else if (strcmp(subcmd, "UPDATE") == 0) {
        a_values[idx] = A_REQUEST_FLAG;
        resp->println(" -> UPDATE");
    } else {
        resp->println(" ???");
    }
}

