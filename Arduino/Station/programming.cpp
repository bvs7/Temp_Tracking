
#include "programming.h"

#include "secrets.h"
#include "settings.h"
#include "station.h"
#include "utility.h"

void print_settings(){

    Serial.println("Printing current settings:");
    Serial.print("VALID: ");
    Serial.println(get_byte(VALID));
    Serial.print("FW_VERSION: ");
    char *setting = get_str(FW_VERSION, SETTING_LEN);
    Serial.println(setting);
    free(setting);
    Serial.print("CATEGORY: ");
    setting = get_str(CATEGORY, SETTING_LEN);
    Serial.println(setting);
    free(setting);
    Serial.print("STATION_NAME: ");
    setting = get_str(STATION_NAME, SETTING_LEN);
    Serial.println(setting);
    free(setting);
    Serial.print("WIFI_SSID: ");
    setting = get_str(WIFI_SSID, SETTING_LEN);
    Serial.println(setting);
    free(setting);
    Serial.print("WIFI_PASSWD: ");
    setting = get_str(WIFI_PASSWD, SETTING_LEN);
    Serial.println(setting);
    free(setting);
    Serial.print("MQTT_SERVER: ");
    setting = get_str(MQTT_SERVER, SETTING_LEN);
    Serial.println(setting);
    free(setting);
    Serial.print("MQTT_PORT: ");
    Serial.println(get_int(MQTT_PORT));
    Serial.print("POLL_INTERVAL: ");
    Serial.println(get_int(POLL_INTERVAL));
    Serial.print("P PINS: ");
    for (int i = 0; i < NUM_P_DEVICES; i++) {
        char sense[4];
        pin_num_to_name(get_byte(P_SENSE(i)), sense);
        Serial.print(sense);
        Serial.print("/");
        char ctrl[4];
        pin_num_to_name(get_byte(P_CTRL(i)), ctrl);
        Serial.print(ctrl);
        Serial.print("; ");
    }
    Serial.println();
    Serial.print("A PINS: ");
    for (int i = 0; i < NUM_A_DEVICES; i++) {
        char input[4];
        pin_num_to_name(get_byte(A_INPUT(i)), input);
        Serial.print(input);
        Serial.print("/");
        char trig[4];
        pin_num_to_name(get_byte(A_TRIG(i)), trig);
        Serial.print(trig);
        Serial.print("; ");
    }
    Serial.println();
    
    Serial.println("End of settings");
}

/**
 * @brief Program the EEPROM with provided settings
 * secrets.h provides connection info
 * station.h provides station settings
 * To prevent overwriting EEPROM, only program if EEPROM is not valid
 *  or if user sends something to Serial within 60 seconds
 */
void program() {
    Serial.begin(9600);

    byte valid = get_byte(VALID);
    if (valid == EEPROM_SETTINGS_VALID) {
        Serial.println("EEPROM already valid");
        Serial.println("Are you sure you want to program the EEPROM? (y/n)");
        while (Serial.available() > 0) {
            char c = Serial.read();
            if (c == 'y') {
                program();
            } else if (c == 'n') {
                return;
            }
        }
    }

    INFO("Write FW version ", FW_VERSION_);
    set_str(FW_VERSION, FW_VERSION_, SETTING_LEN);
    delay(10);

    INFO("Write category ", CATEGORY_);
    set_str(CATEGORY, CATEGORY_, SETTING_LEN);
    delay(10);

    INFO("Write station name ", STATION_NAME_);
    set_str(STATION_NAME, STATION_NAME_, SETTING_LEN);
    delay(10);

    INFO("Write ssid ", WIFI_SSID_);
    set_str(WIFI_SSID, WIFI_SSID_, SETTING_LEN);
    delay(10);

    INFO("Write password ", WIFI_PASSWD_);
    set_str(WIFI_PASSWD, WIFI_PASSWD_, SETTING_LEN);
    delay(10);

    INFO("Write MQTT server ", MQTT_SERVER_);
    set_str(MQTT_SERVER, MQTT_SERVER_, SETTING_LEN);
    delay(10);

    INFO("Write MQTT port ", MQTT_PORT_);
    set_int(MQTT_PORT, MQTT_PORT_);
    delay(10);

    // P devices
    INFO("Write P devices:", "");
    for (int i = 0; i < NUM_P_DEVICES; i++) {
        INFO("  P", i);
        INFO("    sense pin: ", p_sense[i]);
        set_byte(P_SENSE(i), p_sense[i]);
        INFO("    ctrl pin: ", p_ctrl[i]);
        set_byte(P_CTRL(i), p_ctrl[i]);
    }

    // A devices
    INFO("Write A devices:", "");
    for (int i = 0; i < NUM_A_DEVICES; i++) {
        INFO("  A", i);
        INFO("    input pin: ", a_input[i]);
        set_byte(A_INPUT(i), a_input[i]);
        INFO("    trig pin: ", a_trig[i]);
        set_byte(A_TRIG(i), a_trig[i]);
    }

    INFO("Write A poll interval: ", POLL_INTERVAL_);
    set_int(POLL_INTERVAL, POLL_INTERVAL_);

    set_byte(VALID, EEPROM_SETTINGS_VALID);
    Serial.println("Finished Programming!");
}
