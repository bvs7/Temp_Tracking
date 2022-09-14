
#include "programming.h"

#include "station.h"
#include "secrets.h"
#include "settings.h"
#include "utility.h"

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
        Serial.println("Hit Enter to Program...");
        while (!Serial.available()) {
            // clang-format off
            dash(); space(); space(); space();
            // clang-format on
            if (millis() > (1000 * 60)) {
                Serial.println("1 minute without activity, hanging");
                while (1)
                    ;
            }
        }
    }

    INFO("Write FW version ", FW_VERSION_);
    set_str(FW_VERSION, FW_VERSION_, SETTING_LEN);

    INFO("Write station name ", STATION_NAME_);
    set_str(STATION_NAME, STATION_NAME_, SETTING_LEN);

    INFO("Write ssid ", WIFI_SSID_);
    set_str(WIFI_SSID, WIFI_SSID_, SETTING_LEN);

    INFO("Write password ", WIFI_PASSWD_);
    set_str(WIFI_PASSWD, WIFI_PASSWD_, SETTING_LEN);

    INFO("Write MQTT server ", MQTT_SERVER_);
    set_str(MQTT_SERVER, MQTT_SERVER_, SETTING_LEN);

    INFO("Write MQTT port ", MQTT_PORT_);
    set_int(MQTT_PORT, MQTT_PORT_);

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
