#include "connection.h"

#include "utility.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3);  // RX, TX
#endif

#define AT_BAUD_RATE 9600
#define RECONNECT_DELAY_MILLIS 60000

bool disconnect_flag = false;
unsigned long reconnect_time_millis = 0;

WiFiEspClient wifi_client;
PubSubClient mqtt_client(wifi_client);

// TODO: pull out reason for failed connection somehow
// int wifi_begin(const char *ssid, const char *pass) {
//     return EspDrv::sendCmd(F("AT+CWJAP_CUR=\"%s\",\"%s\""), 20000, ssid,
//     pass);
// }

byte wifi_connect(connection_settings *conn) {
    byte status = WiFi.status();

    switch (status) {
        case WL_NO_SHIELD:
        case WL_DISCONNECTED:
            WiFi.init(&Serial1);
            status = WiFi.status();
            if (status == WL_NO_SHIELD) {
                ERROR("ESP8266 not found ", status);
                break;
            }
            INFO("ESP8266 found", "");
            delay(100);
        case WL_IDLE_STATUS:
        case WL_CONNECT_FAILED:
            INFO("Connecting to WiFi network ", conn->ssid);
            delay(100);
            int attempts = 0;
            while (status != WL_CONNECTED) {
                delay(100);
                status = WiFi.begin(conn->ssid, conn->password);
                delay(1000);
                attempts++;
                if (attempts >= 10) {
                    ERROR("Could not connect to WiFi network ", conn->ssid);
                    break;
                }
            }
            if(status == WL_CONNECTED){
                INFO("Successfully connected to WiFi network ", conn->ssid);
            }
            break;
        case WL_CONNECTED:
            INFO("Already connected to WiFi network ", conn->ssid);
            break;
    }
    return status;
}

int8_t mqtt_connect(connection_settings *conn) {
    switch (mqtt_client.state()) {
        case MQTT_CONNECTION_LOST:
        case MQTT_CONNECT_FAILED:
            byte wifi_state = wifi_connect(conn);
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
            mqtt_client.setServer(conn->mqtt_server, conn->mqtt_port);
            INFO("Connecting to MQTT", "");
            INFO("  Server: ", conn->mqtt_server);
            INFO("  Port: ", conn->mqtt_port);
            INFO("  ID: ", conn->mqtt_id);
            mqtt_client.connect(conn->mqtt_id);
            int8_t mqtt_state = mqtt_client.state();
            if (mqtt_state != MQTT_CONNECTED) {
                INFO("Failed to connect to MQTT: ", mqtt_state);
                break;
            }
        case MQTT_CONNECTED:
            INFO("Successfully connected to MQTT server ", conn->mqtt_server);
            break;
    }
}

bool publish(const char *topic, const char *payload, bool retain) {
    if (mqtt_client.publish(topic, payload, retain)) {
        INFO("Published to topic ", topic);
        INFO("  Payload: ", payload);
        return true;
    } else {
        ERROR("Failed to publish to topic ", topic);
        return false;
    }
}

bool publish_data(const char *topic_suffix, const char *payload, bool retain) {
    return publish(strcat("data/irrigation/tank_station1", topic_suffix), payload, retain);
}

bool publish_error(const char *message, bool retain) {
    return publish("error/", message, retain);
}

bool subscribe(const char *topic, int qos) {
    if (mqtt_client.subscribe(topic, qos)) {
        INFO("Subscribed to topic ", topic);
        return true;
    } else {
        ERROR("Failed to subscribe to topic ", topic);
        return false;
    }
}

bool unsubscribe(const char *topic) {
    if (mqtt_client.unsubscribe(topic)) {
        INFO("Unsubscribed from topic ", topic);
        return true;
    } else {
        ERROR("Failed to unsubscribe from topic ", topic);
        return false;
    }
}

PubSubClient *setCallback(MQTT_CALLBACK_SIGNATURE) {
    mqtt_client.setCallback(callback);
    return &mqtt_client;
}

void connection_setup(station_settings *station) {
    Serial1.begin(AT_BAUD_RATE); // Down the road, get a 115200 baud rate

    delay(100);

    WiFi.init(&Serial1);

    byte wifi_status = wifi_connect(&(station->conn));
    if (wifi_status != WL_CONNECTED) {
        return;
    }
    int8_t mqtt_status = mqtt_connect(&station->conn);
}

void connection_loop(station_settings *station) {
    if (!mqtt_client.loop()) {
        if (!disconnect_flag) {
            disconnect_flag = true;
            reconnect_time_millis = millis() + RECONNECT_DELAY_MILLIS;
        }
        if (millis() > reconnect_time_millis) {
            disconnect_flag = false;
            mqtt_connect(&station->conn);
        }
    }
}
