#include "connection.h"

#include "Arduino.h"
#include "settings.h"
#include "utility.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3);  // RX, TX
#endif

#define FILE_ "conn: "

#define AT_BAUD_RATE 9600
#define RECONNECT_DELAY_MILLIS 60000  // 60 seconds

bool connect = true;
bool disconnect_flag = true;
unsigned long reconnect_time_millis = 0;

WiFiEspClient wifi_client;
PubSubClient mqtt_client(wifi_client);

// TODO: pull out reason for failed connection somehow
// int wifi_begin(const char *ssid, const char *pass) {
//     return EspDrv::sendCmd(F("AT+CWJAP_CUR=\"%s\",\"%s\""), 20000, ssid,
//     pass);
// }

byte wifi_connect() {
    byte status = WiFi.status();
    switch (status) {
        case WL_NO_SHIELD:
        case WL_DISCONNECTED:
            WiFi.init(&Serial1);
            status = WiFi.status();
            if (status == WL_NO_SHIELD) {
                ERR(FILE_, __LINE__); //ERROR("WiFi: ", "No Shield");
                delay(100);
                break;
            }
            // Intentional fallthrough
        case WL_IDLE_STATUS:
        case WL_CONNECT_FAILED:
            int attempts = 0;
            char *ssid = get_str(WIFI_SSID, SETTING_LEN);
            char *passwd = get_str(WIFI_PASSWD, SETTING_LEN);
            while (status != WL_CONNECTED) {
                delay(100);
                status = WiFi.begin(ssid, passwd);
                delay(100);
                attempts++;
                if (attempts >= 10) {
                    ERR(FILE_, __LINE__); //ERROR("WiFi: ", "Couldn't Connect");
                    break;
                }
            }
            free(ssid);
            free(passwd);
            if (WiFi.status() != WL_CONNECTED) {
                ERR(FILE_, __LINE__); //ERROR("WiFi: ", "Couldn't Connect");
                break;
            }
            INFO("WiFi: ", "Connected");
            // clang-format off
            dash(); dot(); space();
            dash(); dot(); space();
            dash(); dot(); space();
            // clang-format on
        case WL_CONNECTED:
            break;
    }
    return status;
}

int8_t mqtt_connect() {
    switch (mqtt_client.state()) {
        case MQTT_CONNECTION_LOST:
        case MQTT_CONNECT_FAILED:
        case MQTT_DISCONNECTED:
            if (wifi_connect() != WL_CONNECTED) {
                ERR(FILE_, __LINE__); //ERROR("MQTT: ", "Failed: WiFi");
                break;
            }
            // Intentional fallthrough
        case MQTT_CONNECTION_TIMEOUT:
        case MQTT_CONNECT_BAD_PROTOCOL:
        case MQTT_CONNECT_BAD_CLIENT_ID:
        case MQTT_CONNECT_UNAVAILABLE:
        case MQTT_CONNECT_BAD_CREDENTIALS:
        case MQTT_CONNECT_UNAUTHORIZED:
            char *mqtt_server = get_str(MQTT_SERVER, SETTING_LEN);
            int mqtt_port = get_int(MQTT_PORT);
            mqtt_client.setServer(mqtt_server, mqtt_port);
            mqtt_client.connect(station_name);
            int8_t mqtt_state = mqtt_client.state();
            if (mqtt_state != MQTT_CONNECTED) {
                ERR(FILE_, __LINE__); //ERROR("MQTT: ", "Couldn't Connect");
                break;
            }
            free(mqtt_server);
            // Intentional fallthrough
        case MQTT_CONNECTED:
            INFO("MQTT: ", "Connected");
            char topic[2*SETTING_LEN];
            snprintf(topic, 2*SETTING_LEN, CMD "/%s/#", station_name);
            subscribe(topic, 0);
            // clang-format off
            dash(); dot(); dot(); space();
            dash(); dot(); dot(); space();
            dash(); dot(); dot(); space();
            // clang-format on
            break;
    }
}

bool publish_data(const char *topic_suffix, const char *payload, bool retain) {
    char topic[32];
    snprintf(topic, 32, (DATA "/%s/%s"), station_name, topic_suffix);
    return mqtt_client.publish(topic, payload, retain);
}

bool publish_log(const char *message, bool retain) {
    char topic[32];
    snprintf(topic, 32, (LOG "/%s"), station_name);
    return mqtt_client.publish(topic, message, retain);
}

bool subscribe(const char *topic, int qos) {
    if (mqtt_client.subscribe(topic, qos)) {
        INFO("Subscribed to topic ", topic);
        return true;
    } else {
        ERR(FILE_, __LINE__); //ERROR("Failed to subscribe to topic ", topic);
        return false;
    }
}

bool unsubscribe(const char *topic) {
    if (mqtt_client.unsubscribe(topic)) {
        INFO("Unsubscribed from topic ", topic);
        return true;
    } else {
        ERR(FILE_, __LINE__); //ERROR("Failed to unsubscribe from topic ", topic);
        return false;
    }
}

PubSubClient *set_callback(MQTT_CALLBACK_SIGNATURE) {
    mqtt_client.setCallback(callback);
    return &mqtt_client;
}

void connection_setup() {
    Serial1.begin(AT_BAUD_RATE);

    delay(100);

    WiFi.init(&Serial1); // Necessary here to prevent failure
    // If no shield, stop trying to reconnect
    byte w_status = WiFi.status();
    if (true) {
        ERROR("No WiFi Shield: ", w_status);
        connect = false;
    }
    // byte wifi_status = wifi_connect();
    // int8_t mqtt_status = mqtt_connect();
}

void connection_loop() {
    if(!connect) {
        return;
    }
    if (!mqtt_client.loop()) {
        if (!disconnect_flag) {
            disconnect_flag = true;
            reconnect_time_millis = millis() + RECONNECT_DELAY_MILLIS;
        }
        if (millis() > reconnect_time_millis) {
            WARN("Reconnect", "");
            disconnect_flag = false;
            mqtt_connect();
        }
    }
}
