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

/**
 * @brief Attempt to connect to the WiFi network.
 *
 * @return byte WiFi status
 */
byte wifi_connect() {
    byte status = WiFi.status();
    switch (status) {
        case WL_NO_SHIELD:
            // Try to reinitialize the Serial1 connection?
            Serial1.end();
            Serial1.begin(AT_BAUD_RATE);
        case WL_DISCONNECTED:
            WiFi.init(&Serial1);
            status = WiFi.status();
            if (status == WL_NO_SHIELD) {
                ERR(FILE_, __LINE__);  // ERROR("WiFi: ", "No Shield");
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
                    ERR(FILE_,
                        __LINE__);  // ERROR("WiFi: ", "Couldn't Connect");
                    break;
                }
            }
            free(ssid);
            free(passwd);
            if (WiFi.status() != WL_CONNECTED) {
                ERR(FILE_, __LINE__);  // ERROR("WiFi: ", "Couldn't Connect");
                break;
            }
            INFO("WiFi: ", "Connected");
            // clang-format off
            dash(); dot(); space();
            dash(); dot(); space();
            // clang-format on
        case WL_CONNECTED:
            break;
    }
    return status;
}

/**
 * @brief Attempt to connect to the MQTT broker.
 *
 * @return int8_t MQTT connection status
 */
int8_t mqtt_connect() {
    switch (mqtt_client.state()) {
        case MQTT_CONNECTION_LOST:
        case MQTT_CONNECT_FAILED:
        case MQTT_DISCONNECTED:
            if (wifi_connect() != WL_CONNECTED) {
                ERR(FILE_, __LINE__);  // ERROR("MQTT: ", "Failed: WiFi");
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
                ERR(FILE_, __LINE__);  // ERROR("MQTT: ", "Couldn't Connect");
                break;
            }
            free(mqtt_server);
            // Intentional fallthrough
        case MQTT_CONNECTED:
            INFO("MQTT: ", "Connected");
            char topic[2 * SETTING_LEN];
            snprintf(topic, 2 * SETTING_LEN, "%s/%s/%s/#", CMD, category,
                     station_name);
            subscribe(topic, 0);
            // clang-format off
            Serial.println("Connected!");
            dash(); dot(); dot(); space();
            dash(); dot(); dot(); space();
            // clang-format on
            break;
    }
}

/**
 * @brief Publish a data message to the MQTT broker.
 *
 * @param topic_suffix Topic will be: "data/<station_name>/<topic_suffix>"
 * @return If the message was published successfully.
 */
bool publish_data(const char *topic_suffix, const char *payload, bool retain) {
    char topic[BUFFER_SIZE];
    snprintf(topic, BUFFER_SIZE, (DATA "/%s/%s/%s"), category, station_name,
             topic_suffix);
    WARN("Publish Data: ", topic);
    WARN(" ", payload);
    return mqtt_client.publish(topic, payload, retain);
}

/**
 * @brief Publish a log message to the MQTT broker.
 *      Topic will be "log/<station_name>"
 *
 * @return If the message was published successfully.
 */
bool publish_log(const char *message, bool retain) {
    char topic[BUFFER_SIZE];
    snprintf(topic, BUFFER_SIZE, (LOG "/%s/%s"), category, station_name);
    DEBUG("Publish Log: ", message);
    return mqtt_client.publish(topic, message, retain);
}

/**
 * @brief Subscribe to a topic on the MQTT broker.
 */
bool subscribe(const char *topic, int qos) {
    if (mqtt_client.subscribe(topic, qos)) {
        INFO("Subscribed to topic ", topic);
        return true;
    } else {
        ERR(FILE_, __LINE__);  // ERROR("Failed to subscribe to topic ", topic);
        return false;
    }
}

/**
 * @brief unsubscribe from a topic on the MQTT broker.
 */
bool unsubscribe(const char *topic) {
    if (mqtt_client.unsubscribe(topic)) {
        INFO("Unsubscribed from topic ", topic);
        return true;
    } else {
        ERR(FILE_,
            __LINE__);  // ERROR("Failed to unsubscribe from topic ", topic);
        return false;
    }
}

/**
 * @brief Set callback for when a message is received from the MQTT broker.
 */
PubSubClient *set_callback(MQTT_CALLBACK_SIGNATURE) {
    mqtt_client.setCallback(callback);
    return &mqtt_client;
}

/**
 * @brief Called in main setup()
 */
void connection_setup() {
    Serial1.begin(AT_BAUD_RATE);

    WiFi.init(&Serial1);  // Necessary here to prevent failure
    // If no shield, stop trying to reconnect
    byte w_status = WiFi.status();
    if (w_status == WL_NO_SHIELD) {
        ERROR("No WiFi Shield: ", w_status);
        connect = false;
    }
    // 5:01 keepalive, analog inputs will preempt this
    mqtt_client.setKeepAlive(get_int(POLL_INTERVAL)+1);
}

/**
 * @brief Called in main loop()
 */
void connection_loop() {
    if (!connect) {
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
