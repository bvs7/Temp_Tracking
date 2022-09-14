
#include "settings.h"

// #define PROGRAMMING_MODE
#ifndef PROGRAMMING_MODE

#include "commands.h"
#include "connection.h"
#include "devices.h"
#include "utility.h"

// library includes
#include <avr/wdt.h>

#include "LoopbackStream.h"

#define FW_VERSION_ "0.0.1"

#define FILE_ "main: "

// clang-format off
char state_str[8][10] = {
//  ctrl off   ctrl on
    "DISABLED","IDLE!",  // sense off
    "FORCED!", "ENABLED",// sense on

    "*DISABLED","*IDLE!", // request flag
    "*FORCED!", "*ENABLED",
};  // clang-format on

char category[SETTING_LEN];
char station_name[SETTING_LEN];

p_state p_states[NUM_P_DEVICES] = {0};
a_value a_values[NUM_A_DEVICES];

LoopbackStream SerialIn(BUFFER_SIZE);
LoopbackStream MQTTOut(BUFFER_SIZE);

/**
 * @brief Handle an mqtt message, MQTT_CALLBACK_SIGNATURE structure
 *
 * @param topic mqtt topic, should be "cmd/<station_name>/[<device_name>]"
 * @param payload Either a root command or details for a device command
 */
void mqtt_callback(char *topic, byte *payload, unsigned int length) {
    mqtt_handle(topic, payload, length, &MQTTOut);
    publish_log(MQTTOut.readString().c_str(), false);
}

/**
 * @brief Handle a command from Serial
 * Serial input is passed to SerialIn LoopbackStream. Upon '\n', handle command
 */
void serial_callback() {
    char input_buffer[BUFFER_SIZE] = {0};
    int len = SerialIn.readBytesUntil('\n', input_buffer, BUFFER_SIZE);
    if (len >= BUFFER_SIZE) {
        ERR(FILE_, __LINE__);  // ERROR("Serial input too long");
        return;
    }
    return root_handle(input_buffer, &Serial);
}

void setup() {
    Serial.begin(9600);
    wdt_disable();
    INFO("Start", "");
    char *fw_version = get_str(FW_VERSION, SETTING_LEN);
    if (strcmp(fw_version, FW_VERSION_) != 0) {
        Serial.print("EEPROM FW version mismatch: ");
        Serial.print(fw_version);
        Serial.print(" != ");
        Serial.println(FW_VERSION_);
        Serial.print("Hanging...");
        while (1)
            ;
    }
    util_setup();
    byte valid = get_byte(VALID);
    if (valid != EEPROM_SETTINGS_VALID) {
        ERROR("EEPROM not valid", "");
        while (1)
            ;
    }

    char *category_tmp = get_str(CATEGORY, SETTING_LEN);
    strcpy(category, category_tmp);
    free(category_tmp);
    char *station_name_tmp = get_str(STATION_NAME, SETTING_LEN);
    strcpy(station_name, station_name_tmp);
    free(station_name_tmp);

    connection_setup();
    set_callback(mqtt_callback);
    devices_setup();
}

void loop() {
    connection_loop();
    devices_loop();

    while (Serial.available() > 0) {
        char c = Serial.read();
        SerialIn.write(c);
        if (c == '\n') {
            serial_callback();
        }
    }
}

#else

#include "programming.h"

#include "utility.h"

void setup() {
    util_setup();
    program();
}

void loop() {
    // clang-format off
    {dash(); dash(); dash(); space();} // O
    {dash(); dot();  dash(); space();} // K
    {space(); space(); space();} // _
    // clang-format on
}
#endif
