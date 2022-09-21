#include "devices.h"

#include "Arduino.h"
#include "connection.h"
#include "settings.h"
#include "utility.h"

#include "PubSubClient.h"

unsigned long last_tick_millis = 0;
unsigned long seconds = 0;

/**
 * @brief Blocking fn to get distance from sensor.
 *
 * @return int Distance in milimeters
 */
int dist_sensor(int trig_pin, int echo_pin) {
    // Make sure that trigger pin is LOW.
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure
    // distance.
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

// Speed of sound is 343 m/s or .000343 mm/ns.
// Inverse speed of sound is 2915 ns/mm.
#define NS_PER_MM 2915
#define NS_PER_US 1000
#define MAX_DISTANCE_MM 4000
#define MAX_DURATION_US (MAX_DISTANCE_MM * 2 * NS_PER_MM / NS_PER_US)

    unsigned long duration_us = pulseIn(echo_pin, HIGH, MAX_DURATION_US);

    return (int)(duration_us * NS_PER_US / NS_PER_MM / 2);
}

/**
 * @brief Set a port to an ON/OFF ctrl state
 *
 * @param idx Port Number
 * @param set true for ON, false for OFF
 */
void p_ctrl_set(byte idx, bool set) {
    if (set) {
        digitalWrite(get_byte(P_CTRL(idx)), HIGH);
    } else {
        digitalWrite(get_byte(P_CTRL(idx)), LOW);
    }
}

/**
 * @brief Check if port state has changed and update if so
 *
 * @param idx Port Number
 */
void update_p_device(uint8_t idx) {
    byte sense = get_byte(p[idx].sense);
    if (sense == UNUSED_PIN) {
        return;
    }
    byte ctrl = get_byte(P_CTRL(idx));
    p_state new_state = (digitalRead(sense) ? 0 : SENSE_MASK) |
                        (digitalRead(ctrl) ? CTRL_MASK : 0);
    if (p_states[idx] != new_state) {
        WARN("Update P", idx);
        WARN("", state_str[new_state]);
        p_states[idx] = new_state;
        char topic[9] = "p0/state";
        topic[1] += idx;
        char value_str[2] = "0";
        publish(topic, itoa(new_state,value_str,10), true);
    }
}

/**
 * @brief Update the pin modes for a port
 *      TODO: Keep track of pin use and switch duplicate pins to UNUSED_PIN
 * @param idx Port Number
 */
void p_device_setup(uint8_t idx) {
    VERBOSE("Setting up P", idx);
    byte sense = get_byte(P_SENSE(idx));
    if (sense == UNUSED_PIN) {
        return;
    }
    byte ctrl = get_byte(P_CTRL(idx));
    // TODO: check pin conflicts
    pinMode(sense, INPUT);
    pinMode(ctrl, OUTPUT);
    digitalWrite(ctrl, get_byte(p_states[idx]) & CTRL_MASK);
    p_states[idx] |= P_REQUEST_FLAG;
}

typedef PubSubClient MQTTclient;

void homie_setup(char *category, char *station_name){

    char topic[40]; // topic: "homie/device_id/"
    snprintf(topic, 40, "%s/%s/", category, station_name);
    char *setting = topic + strlen(topic);
    publish_homie_sett(topic, setting, "$homie", "4.0.0");
    publish_homie_sett(topic, setting, "$state", "init");
    publish_homie_sett(topic, setting, "$name", station_name);
    publish_homie_sett(topic, setting, "$nodes", "p0,p2");
    publish_homie_sett(topic, setting, "$extensions", "");

    for(uint8_t i=0; i<5; i+=1){
        if(get_byte(P_SENSE(i)) == UNUSED_PIN){
            continue;
        }
        char node[4] = "p0"; // Extra space for '/'
        node[1] += i;
        if (i == 0){
            homie_pnode_setup(node, topic, setting, "pump");
        }else{
            homie_pnode_setup(node, topic, setting, "valve");
        }
    }

    publish_homie_sett(topic, setting, "$state", "ready");

    strcpy(setting, "+/+/set"); // topic: "homie/device_id/+/+/set"
    subscribe(topic, 1);
}

void homie_pnode_setup(char *n_id, char *topic, char *sett, char *n_type){

    strcpy(sett, n_id); // topic: "homie/device_id/pN"
    char *n_setting = topic + strlen(topic);
    strcpy(n_setting, "/");   // topic: "homie/device_id/pN/"
    n_setting = topic + strlen(topic);
    publish_homie_sett(topic, n_setting, "$name", n_id);
    publish_homie_sett(topic, n_setting, "$type", n_type);
    publish_homie_sett(topic, n_setting, "$properties", "ctrl,state");

    strcpy(n_setting, "ctrl/"); // topic: "homie/device_id/pN/ctrl/"
    char *p_setting = topic + strlen(topic);
    publish_homie_sett(topic, p_setting, "$name", "ctrl");
    publish_homie_sett(topic, p_setting, "$datatype", "boolean");
    publish_homie_sett(topic, p_setting, "$settable", "true");

    strcpy(n_setting, "state/"); // topic: "homie/device_id/pN/state/"
    p_setting = topic + strlen(topic);
    publish_homie_sett(topic, p_setting, "$name", "state");
    publish_homie_sett(topic, p_setting, "$datatype", "integer");
    publish_homie_sett(topic, p_setting, "$format", "0:7");

    strcpy(sett, ""); // Reset topic to "homie/device_id/"
}

void publish_homie_sett(char *base_topic, char *sett_loc,
                        const char *sett_name, const char *sett_value){
    strcpy(sett_loc, sett_name);
    publish(base_topic, sett_value, true);
}

/**
 * @brief Set up Devices. Port sense/ctrl and sensor input/trig pins
 */
void devices_setup() {
    VERBOSE("Devices setup", "");

    homie_setup(category, station_name);
    // Reset pin conflicts
    // TODO: Consider conflicts from utility.h, LED_PIN and DEBUG_LED_ENABLE
    for (uint8_t i = 0; i < NUM_P_DEVICES; i++) {
        p_device_setup(i);
    }

    last_tick_millis = millis();
}

void sec_loop(unsigned long seconds) {
    for (uint8_t i = 0; i < NUM_P_DEVICES; i++) {
        update_p_device(i);
    }

}

/**
 * @brief Update devices
 *      Upon a one second tick,
 *          Check for poll_interval and update all sensors if needed
 *          Check for port updates
 *          Check for sensor update requests
 */
void devices_loop() {
    VERBOSE2("Devices loop", "");
    unsigned long current_millis = millis();
    if (current_millis - last_tick_millis > SEC) {
        VERBOSE("Tick ", seconds);
        dot();
        
        seconds += 1;
        while (current_millis - last_tick_millis > SEC) {
            last_tick_millis += SEC;
        }
    }
}
