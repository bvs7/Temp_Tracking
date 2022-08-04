
#include "IrrigationStation.h"

#include "EEPROM.h"

#define P0_CTRL 13
#define P0_SENSE 5
#define P1_CTRL 6
#define P1_SENSE 7
#define P2_CTRL 8
#define P2_SENSE 9
#define P3_CTRL 10
#define P3_SENSE 11
#define P4_CTRL 12
#define P4_SENSE 14 // Warning! LED_BUILTIN is 13, 14 if unused

#define DEBUG_BAUD_RATE 9600
#define BUFFER_SIZE 64

unsigned long heartbeat_alarm_millis = 0;
#define HEARTBEAT_DELAY_MILLIS 1000

unsigned int buffer_idx = 0;
char buffer[BUFFER_SIZE];

device *devices[NUM_DEVICES];
station_settings station;

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';
    mqtt_callback(topic, message);
}

void mqtt_callback(char* topic, char* message) {
    INFO("Received message on topic ", topic);
    INFO("  Payload: ", message);

    // Check if topic starts with CMD_STATION_TOPIC
    if (strncmp(topic, CMD_STATION_TOPIC, strlen(CMD_STATION_TOPIC)) == 0) {

        if(strlen(topic) == strlen(CMD_STATION_TOPIC)) {
            // handle message as command
            char response[BUFFER_SIZE];
            if (handle_command(message, response, true)) {
                publish_data("", response, false);
            }
        }else{
            // handle remaining topic
            char* topic_suffix = topic + strlen(CMD_STATION_TOPIC);
            char response[BUFFER_SIZE];
            char command[BUFFER_SIZE];
            sprintf(command, "%s %s", topic_suffix, message);
            if (handle_command(command, response, true)) {
                publish_data(topic_suffix, response, false);
            }
        }
    }
}

bool handle_command(char* input, char* response, bool mqtt) {
    response[0] = '\0';
    INFO("Received command: ", input);
    char* saveptr;

    char* cmd = strtok_r(input, " ", &saveptr);
    if (strcmp(cmd, "echo") == 0) {
        DEBUG("Echoing input: ", saveptr);
        strcpy(response, saveptr);
    } else if (strcmp(cmd, "status") == 0) {
        DEBUG("Fetching status", "");
        for (int i = 0; i < NUM_DEVICES; i++) {
            sprintf(response, "%s%s%s%s\n", response, station.device[i].name,
                    ": ", state_str[station.device[i].state]);
        }
    } else if (strcmp(cmd, "mqtt") == 0) {
        DEBUG("Test MQTT: ", saveptr);
        char* topic = strtok_r(NULL, " ", &saveptr);
        mqtt_callback(topic, saveptr);
    } else if (strcmp(cmd, "save") == 0) {
        DEBUG("Saving settings", "");
        save_settings();
        strcpy(response, "OK");
    } else {
        // Check if command is for a device
        for (int i = 0; i < NUM_DEVICES; i++) {
            if (strcmp(cmd, station.device[i].name) == 0) {
                DEBUG("Command for device ", cmd);
                char* action = strtok_r(NULL, " ", &saveptr);
                if (strcmp(action, state_str[DEVICE_STATE_ON]) == 0) {
                    DEBUG("Turning on device ", cmd);
                    devices[i]->set_ctrl(HIGH);
                    strcpy(response, "OK");
                    return true;
                } else if (strcmp(action, state_str[DEVICE_STATE_OFF]) == 0) {
                    DEBUG("Turning off device ", cmd);
                    devices[i]->set_ctrl(LOW);
                    strcpy(response, "OK");
                    return true;
                } else if (action == NULL) {
                    DEBUG("Pushing device state", "");
                    strcpy(response,state_str[station.device[i].state]);
                    return true;
                } else {
                    if(mqtt){
                        publish_error("Unknown action", true);
                    }else{
                        ERROR("Unknown action ", action);
                    }
                    return false;
                }
            }
        }
        if(mqtt){
            publish_error("Unknown command", true);
        }else{
            ERROR("Unknown command ", cmd);
        }
        return false;
    }
    return true;
}

void handle_serial() {
    unsigned long start = millis();
    while (Serial.available() && buffer_idx < BUFFER_SIZE && (millis() - start) < 100) {
        char c = Serial.read();
        if (c == '\n') {
            buffer[buffer_idx] = '\0';
            char response[BUFFER_SIZE] = {};
            if(handle_command(buffer, response, false)){
              Serial.println(response);
            }
            buffer_idx = 0;
            break;
        }
        buffer[buffer_idx++] = c;
    }
}

void save_settings() {
    station.valid = EEPROM_SETTINGS_VALID;
    EEPROM.put(STATION_SETTINGS_ADDR_START, station);
}

void load_settings() {
    byte valid = EEPROM.read(STATION_SETTINGS_ADDR_START);
    if (valid == EEPROM_SETTINGS_VALID) {
        DEBUG("Settings valid. Loading from EEPROM", "");
        EEPROM.get(STATION_SETTINGS_ADDR_START, station);
    } else {
        WARN("Settings invalid. Settings not initialized", "");
        strcpy(station.device[0].name,"P0");
        strcpy(station.device[1].name,"P1");
        strcpy(station.device[2].name,"P2");
        strcpy(station.device[3].name,"P3");
        strcpy(station.device[4].name,"P4");
    }
}

void setup() {
    Serial.begin(DEBUG_BAUD_RATE);
    Serial.println("Start");
    util_setup();
    load_settings();
    connection_setup(&station);

    static device devices_[NUM_DEVICES] = {
      device(P0_SENSE, P0_CTRL, &station.device[0]),
      device(P1_SENSE, P1_CTRL, &station.device[1]),
      device(P2_SENSE, P2_CTRL, &station.device[2]),
      device(P3_SENSE, P3_CTRL, &station.device[3]),
      device(P4_SENSE, P4_CTRL, &station.device[4])
     };
     for(int i=0; i < NUM_DEVICES; i++){
        devices[i] = &devices_[i];
     }
}

void loop() {
    sizeof(station_settings);
    connection_loop(&station);
    handle_serial();
    if(millis() >= heartbeat_alarm_millis){
        heartbeat_alarm_millis = millis() + HEARTBEAT_DELAY_MILLIS;
        dot();
        for (int i = 0; i < NUM_DEVICES; i++) {
            if(devices[i]->loop_1s()){
                publish_data(station.device[i].name, state_str[station.device[i].state], true);
                INFO("Published data for device ", station.device[i].name);
                INFO("  State: ", state_str[station.device[i].state]);
            }
        }
    }
}
