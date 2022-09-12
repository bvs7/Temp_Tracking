
#include "settings.h"

// #define PROGRAMMING_MODE

#ifndef PROGRAMMING_MODE

#include "utility.h"

#include "commands.h"
#include "connection.h"
#include "devices.h"

// library includes
#include "LoopbackStream.h"

char state_str[8][11] = {"OFF+UNSET",  "OFF!SET",  "ON!UNSET",  "ON+SET",
                         "*OFF+UNSET", "*OFF!SET", "*ON!UNSET", "*ON+SET",};

char *station_name;

p_state p_states[NUM_P_DEVICES] = {0};
a_value a_values[NUM_A_DEVICES];

#define BUFFER_SIZE 32

LoopbackStream SerialIn(BUFFER_SIZE);
LoopbackStream MQTTOut(BUFFER_SIZE);

void mqtt_callback(char *topic, byte *payload, unsigned int length) {
    mqtt_handle(topic, payload, length, &MQTTOut);
    publish_log(MQTTOut.readString().c_str(), false);
}

void serial_callback() {
    DEBUG2("Serial callback", "");
    char input_buffer[BUFFER_SIZE] = {0};
    SerialIn.readBytesUntil('\n', input_buffer, BUFFER_SIZE);
    return root_handle(input_buffer, &Serial);
}

void setup() {
    Serial.begin(9600);
    DEBUG("Starting...", "");

    util_setup();

    byte valid = get_byte(VALID);
    if (valid != EEPROM_SETTINGS_VALID) {
        ERROR("EEPROM not valid", "");
        return;
    }
    station_name = get_str(STATION_NAME, SETTING_LEN);

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
    {dash(); dash(); dash(); space();}
    {dash(); dot();  dash(); space();}
    {space(); space(); space();}
    // clang-format on
}
#endif
