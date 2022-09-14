
#include "commands.h"

#include "Arduino.h"
#include "connection.h"
#include "devices.h"
#include "settings.h"
#include "utility.h"

// Library includes
#include <avr/wdt.h>

#define FILE_ "comm: "

/**
 * @brief Convert pin string to Arduino pin number
 *
 * @param pin pin name e.g. "D0", "A3", "12", "-"
 * @return uint8_t pin number e.g. 0, 3, 12, UNUSED_PIN
 */
uint8_t pin_name_to_num(char *pin) {
    if (pin[0] == 'A') {
        return atoi(pin + 1) + 14;
    } else if (pin[0] == '-') {
        return UNUSED_PIN;
    } else {
        return atoi(pin);
    }
}

/**
 * @brief Convert Arduino pin number to pin string
 *
 * @param pin pin number e.g. 0, 3, 12, UNUSED_PIN
 * @param name pin name e.g. "D0", "A3", "D12", "-"
 */
void pin_num_to_name(uint8_t pin, char *name) {
    if (pin > 13) {
        sprintf(name, "A%d", pin - 14);
    } else if (pin == UNUSED_PIN) {
        sprintf(name, "-");
    } else {
        sprintf(name, "D%d", pin);
    }
}

/**
 * @brief Handle a command from an MQTT message
 *
 * @param topic Either "cmd/<station_name>" or "cmd/<station_name>/<device>"
 *           "cmd/<station_name>": payload is root command
 *           "cmd/<station_name>/<device>": payload is device command
 * @param resp  MQTTOut LoopbackStream for log response
 */
void mqtt_handle(char *topic, byte *payload, unsigned int length,
                 Stream *resp) {
    char *saveptr = NULL;
    char *mode = strtok_r(topic, "/", &saveptr);
    if (mode == NULL || strcmp(mode, CMD) != 0) {
        ERR(FILE_, __LINE__);  // ERROR("Invalid topic: ", mode == NULL ? "NULL"
                               // : mode);
        return;
    }
    char *topic_category = strtok_r(NULL, "/", &saveptr);
    if (topic_category == NULL || strcmp(topic_category, category) != 0) {
        ERR(FILE_, __LINE__);  // ERROR("Invalid topic: ", category == NULL ?
                               // "NULL" : category);
        return;
    }
    char *name = strtok_r(NULL, "/", &saveptr);
    if (name == NULL || strcmp(name, station_name) != 0) {
        ERR(FILE_, __LINE__);  // ERROR("Invalid name: ", name == NULL ? "NULL"
                               // : name);
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
                ERR(FILE_,
                    __LINE__);  // ERROR("Invalid P device #: ", device_name);
                return;
            }
            p_device_handle(p_id, device_name, (char **)&payload, resp);
        } else if (device_name[0] == 'A') {
            uint8_t a_id = device_name[1] - '0';
            if (a_id >= NUM_A_DEVICES) {
                ERR(FILE_,
                    __LINE__);  // ERROR("Invalid A device #: ", device_name);
                return;
            }
            a_device_handle(a_id, device_name, (char **)&payload, resp);
        } else {
            ERR(FILE_,
                __LINE__);  // ERROR("Invalid device name: ", device_name);
            return;
        }
    }
}

/**
 * @brief Basic command handling
 * Commands:
 * - "hello": respond with "hello"
 * - "P[n]": device command"
 * - "A[n]": device command"
 * - "sync": reset seconds counter
 * - "mqtt": test mqtt command
 * - "version/name": get firmware version or station name
 * - "ssid/password/mqtt_server/mqtt_port/poll": get/set setting
 * - "reboot": reset the device
 *
 * @param input Input string command from Serial or MQTT
 * @param resp  Stream for log response
 */
void root_handle(char *input, Stream *resp) {
    char *saveptr = NULL;
    char *cmd = strtok_r(input, " ", &saveptr);
    if (cmd == NULL) {
        // Empty? What to do?
        return;
    }
    if (strcmp(cmd, "hello") == 0) {
        resp->println("Hello!");
    } else if (cmd[0] == 'P') {
        uint8_t id = cmd[1] - '0';
        if (id < 0 || id >= NUM_P_DEVICES) {
            ERR(FILE_, __LINE__);  // ERROR("Invalid P ID", "");
            return;
        } else {
            p_device_handle(id, cmd, &saveptr, resp);
        }
    } else if (cmd[0] == 'A') {
        uint8_t id = cmd[1] - '0';
        if (id < 0 || id >= NUM_A_DEVICES) {
            ERR(FILE_, __LINE__);  // ERROR("Invalid A ID", "");
            return;
        } else {
            a_device_handle(id, cmd, &saveptr, resp);
        }
    } else if (strcmp(cmd, "sync") == 0) {
        seconds = 0;
    } else if (strcmp(cmd, "mqtt") == 0) {
        char *topic = strtok_r(NULL, " ", &saveptr);
        if (topic == NULL) {
            ERR(FILE_, __LINE__);  // ERROR("No topic specified", "");
            return;
        }
        byte *payload = (byte *)strtok_r(NULL, "\0", &saveptr);
        unsigned int length = strlen((char *)payload);
        mqtt_handle(topic, payload, length, resp);
    } else if (settings_handle(cmd, &saveptr, resp)) {
    } else if (strcmp(cmd, "reboot") == 0) {
        // TODO: Figure out watchdog timer
        resp->println("Rebooting...");
        wdt_enable(WDTO_2S);
    } else {
        resp->println(" ???");
    }
}

/**
 * @brief Handle a command for a named setting
 *
 * @param sett Name of setting e.g. version/name/ssid/password/
 *              mqtt_server/mqtt_port/poll)
 * @param saveptr Pointer to remaining string after sett
 * @param resp Stream for log response
 * @return true if Command was handled
 */
bool settings_handle(char *sett, char **saveptr, Stream *resp) {
    // clang-format off
    char sett_names[][12] = {
        "version", "name", // Cannot set
        "ssid", "password", "mqtt_server", 
        "mqtt_port", "poll" // int, not string
    };
    // clang-format on

    for (uint8_t i = 0; i < sizeof(sett_names) / sizeof(sett_names[0]); i++) {
        if (strcmp(sett, sett_names[i]) == 0) {
            char *value = strtok_r(NULL, " ", saveptr);
            int addr = FW_VERSION + SETTING_LEN * i;
            if (value == NULL) {
                // Get
                resp->print(sett);
                resp->print(": ");
                if (i > 4) {
                    // mqtt_port and poll are int
                    resp->println(get_int(addr));
                } else {
                    char *read = (char *)get_str(addr, SETTING_LEN);
                    resp->println(read);
                    free(read);
                }
            } else {
                // Set
                if (i <= 1) {
                    // Cannot set fw version or name
                    ERR(FILE_, __LINE__);  // ERROR("Cannot set ", sett);
                    return true;
                }
                resp->print(sett);
                resp->print(" -> ");
                if (i > 4) {
                    // mqtt_port and poll are int
                    set_int(addr, atoi(value));
                    resp->println(get_int(addr));
                } else {
                    set_str(addr, value, SETTING_LEN);
                    char *read = (char *)get_str(addr, SETTING_LEN);
                    resp->println(read);
                    free(read);
                }
            }
            return true;
        }
    }
    return false;
}

/**
 * @brief Handle a command for a Port device
 *
 * @param idx Index of device
 * @param device Name of device e.g. P0
 * @param saveptr Remaining command string
 * @param resp Stream for log response
 */
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
            char name[4];
            pin_num_to_name(get_byte(P_SENSE(idx)), name);
            resp->println(name);
        } else {
            set_byte(P_SENSE(idx), pin_name_to_num(value));
            resp->print(".sense -> ");
            char name[4];
            pin_num_to_name(get_byte(P_SENSE(idx)), name);
            resp->println(name);
            devices_setup();
        }
    } else if (strcmp(subcmd, "ctrl") == 0) {
        if (value == NULL) {
            resp->print(".ctrl: ");
            char name[4];
            pin_num_to_name(get_byte(P_CTRL(idx)), name);
            resp->println(name);
        } else {
            digitalWrite(get_byte(P_CTRL(idx)), LOW);
            set_byte(P_CTRL(idx), pin_name_to_num(value));
            resp->print(".ctrl -> ");
            char name[4];
            pin_num_to_name(get_byte(P_CTRL(idx)), name);
            resp->println(name);
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

/**
 * @brief Handle a command for a Sensor device
 *
 * @param idx Index of device
 * @param device Name of device e.g. A0
 * @param saveptr Remaining command string
 * @param resp Stream for log response
 */
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
