
#include "programming.h"

void programming_setup(){

    station_settings settings = {
        .meta = { EEPROM_SETTINGS_VALID, FW_VERSION },
        .config = { 
            .connection = {WIFI_SSID,WIFI_PASSWORD,MQTT_SERVER,STATION_NAME,MQTT_PORT},
            .P = {P0_CONFIG, P1_CONFIG, P2_CONFIG, P3_CONFIG, P4_CONFIG},
            .a_device_poll_interval = POLL_INTERVAL,
            .A = {A0_CONFIG,A1_CONFIG,A2_CONFIG,A3_CONFIG
                /*,A4_CONFIG,A5_CONFIG,A6_CONFIG,A7_CONFIG*/}
        },
        .state = {
            .P_state = {P0_STATE,P1_STATE,P2_STATE,P3_STATE,P4_STATE},
            .A_value = {A0_VALUE,A1_VALUE,A2_VALUE,A3_VALUE
                /*,A4_VALUE,A5_VALUE,A6_VALUE,A7_VALUE*/}
        }
    };

    // Save settings

    save_settings(&settings);
}
