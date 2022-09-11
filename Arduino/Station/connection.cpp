#include "Arduino.h"

#include "utility.h"
#include "settings.h"
#include "connection.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3);  // RX, TX
#endif

#define AT_BAUD_RATE 9600
#define RECONNECT_DELAY_MILLIS 60000 // 60 seconds

bool disconnect_flag = false;
unsigned long reconnect_time_millis = 0;

connection_config conn; // Permanent copy of connection config

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
                // Error
                break;
            }
            delay(100);
        case WL_IDLE_STATUS:
        case WL_CONNECT_FAILED:
            delay(100);
            int attempts = 0;
            while (status != WL_CONNECTED) {
                delay(100);
                status = WiFi.begin(conn.ssid, conn.password);
                delay(1000);
                attempts++;
                if (attempts >= 10) {
                    Serial.println("ERROR:");
                    Serial.println("WiFi");
                    break;
                }
            }
            if(WiFi.status() == WL_CONNECTED){
              dash(); space(); dash(); dash();
            }
            break;
        case WL_CONNECTED:
            break;
    }
    return status;
}

int8_t mqtt_connect() {
    switch (mqtt_client.state()) {
        case MQTT_CONNECTION_LOST:
        case MQTT_CONNECT_FAILED:
            byte wifi_state = wifi_connect();
            if (wifi_state != WL_CONNECTED) {
                break;
            }
        case MQTT_DISCONNECTED:
        case MQTT_CONNECTION_TIMEOUT:
        case MQTT_CONNECT_BAD_PROTOCOL:
        case MQTT_CONNECT_BAD_CLIENT_ID:
        case MQTT_CONNECT_UNAVAILABLE:
        case MQTT_CONNECT_BAD_CREDENTIALS:
        case MQTT_CONNECT_UNAUTHORIZED:
            mqtt_client.setServer(conn.mqtt_server, conn.mqtt_port);
            mqtt_client.connect(conn.station_name);
            int8_t mqtt_state = mqtt_client.state();
            if (mqtt_state != MQTT_CONNECTED) {
                Serial.println("ERROR:");
                Serial.println("MQTT");
                break;
            }
            dot(); dot(); dot();
        case MQTT_CONNECTED:
            INFO("MQTT connected", "");
            break;
    }
}

bool publish_data(const char *topic_suffix, const char *payload, bool retain) {
    char topic[64];
    snprintf(topic, 64, (DATA TOPIC_INFIX "%s/%s"), conn.station_name,
             topic_suffix);
    return mqtt_client.publish(topic, payload, retain);
}

bool publish_log(const char *message, bool retain) {
    char topic[64];
    snprintf(topic, 64, (LOG TOPIC_INFIX "%s"), conn.station_name);
    return mqtt_client.publish(topic, message, retain);
}

bool subscribe(const char *topic, int qos) {
    if (mqtt_client.subscribe(topic, qos)) {
        INFO("Subscribed to topic ", topic);
        return true;
    } else {
        // Error
        return false;
    }
}

bool unsubscribe(const char *topic) {
    if (mqtt_client.unsubscribe(topic)) {
        INFO("Unsubscribed from topic ", topic);
        return true;
    } else {
        // Error
        return false;
    }
}

PubSubClient *set_callback(MQTT_CALLBACK_SIGNATURE) {
    mqtt_client.setCallback(callback);
    return &mqtt_client;
}

void connection_setup(connection_config *c) {
    // Copy once and use permanently. Reboot to change.
    strcpy(conn.ssid, c->ssid);
    strcpy(conn.password, c->password);
    strcpy(conn.mqtt_server, c->mqtt_server);
    strcpy(conn.station_name, c->station_name);
    conn.mqtt_port = c->mqtt_port;

    Serial1.begin(AT_BAUD_RATE);

    delay(100);

    WiFi.init(&Serial1);
    // Initial wifi connection
    byte wifi_status = wifi_connect();
    if (wifi_status != WL_CONNECTED) {
        // Error
        return;
    }
    delay(100);
    int8_t mqtt_status = mqtt_connect();
}

void connection_loop() {

    if (!mqtt_client.loop()) {
        if (!disconnect_flag) {
            disconnect_flag = true;
            reconnect_time_millis = millis() + RECONNECT_DELAY_MILLIS;
        }
        if (millis() > reconnect_time_millis) {
            disconnect_flag = false;
            mqtt_connect();
        }
    }
}
