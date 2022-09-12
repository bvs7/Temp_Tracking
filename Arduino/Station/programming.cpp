
#include "programming.h"

#include "garden.h"
#include "secrets.h"
#include "settings.h"
#include "utility.h"

void program() {
    Serial.begin(9600);
    DEBUG("Hit Enter to Program...", "");

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

    DEBUG2("Write FW version ", FW_VERSION_);
    set_str(FW_VERSION, FW_VERSION_, SETTING_LEN);

    DEBUG2("Write station name ", STATION_NAME_);
    set_str(STATION_NAME, STATION_NAME_, SETTING_LEN);

    DEBUG2("Write ssid ", WIFI_SSID_);
    set_str(WIFI_SSID, WIFI_SSID_, SETTING_LEN);

    DEBUG2("Write password ", WIFI_PASSWD_);
    set_str(WIFI_PASSWD, WIFI_PASSWD_, SETTING_LEN);

    DEBUG2("Write MQTT server ", MQTT_SERVER_);
    set_str(MQTT_SERVER, MQTT_SERVER_, SETTING_LEN);

    DEBUG2("Write MQTT port ", MQTT_PORT_);
    set_int(MQTT_PORT, MQTT_PORT_);

    // P devices
    DEBUG2("Write P devices:", "");
    for (int i = 0; i < NUM_P_DEVICES; i++) {
        DEBUG2("  P", i);
        DEBUG2("    sense pin: ", p_sense[i]);
        set_byte(P_SENSE(i), p_sense[i]);
        DEBUG2("    ctrl pin: ", p_ctrl[i]);
        set_byte(P_CTRL(i), p_ctrl[i]);
    }

    // A devices
    DEBUG2("Write A devices:", "");
    for (int i = 0; i < NUM_A_DEVICES; i++) {
        DEBUG2("  A", i);
        DEBUG2("    input pin: ", a_input[i]);
        set_byte(A_INPUT(i), a_input[i]);
        DEBUG2("    trig pin: ", a_trig[i]);
        set_byte(A_TRIG(i), a_trig[i]);
    }

    DEBUG2("Write A poll interval: ", A_POLL_INTERVAL_);
    set_int(A_POLL_INTERVAL, A_POLL_INTERVAL_);

    set_byte(VALID, EEPROM_SETTINGS_VALID);
    DEBUG("Finished Programming!", "");
}
