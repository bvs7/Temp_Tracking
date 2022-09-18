#include "Arduino.h"
#include "connection.h"
#include "settings.h"
#include "utility.h"

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
void p_device_set(uint8_t idx, bool set) {
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
    byte sense = get_byte(P_SENSE(idx));
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
        char topic[3] = "P0";
        topic[1] += idx;
        publish_data(topic, state_str[new_state], true);
    }
}

/**
 * @brief Get sensor reading and send update
 *
 * @param idx Sensor Number
 */
void update_a_device(uint8_t idx) {
    byte input = get_byte(A_INPUT(idx));
    if (input == UNUSED_PIN) {
        return;
    }
    int new_value = a_values[idx];
    byte trig = get_byte(A_TRIG(idx));
    if (trig == UNUSED_PIN) {
        new_value = analogRead(input);
    } else {
        new_value = dist_sensor(trig, input);
    }
    a_values[idx] = new_value;
    char topic[3] = "A0";
    topic[1] += idx;
    char value_str[6];
    publish_data(topic, itoa(new_value, value_str, 10), true);
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

/**
 * @brief Update the pin modes for a port
 *      TODO: Keep track of pin use and switch duplicate pins to UNUSED_PIN
 * @param idx Sensor Number
 */
void a_device_setup(uint8_t idx) {
    byte input = get_byte(A_INPUT(idx));
    if (input == UNUSED_PIN) {
        return;
    }
    byte trig = get_byte(A_TRIG(idx));
    // TODO: check pin conflicts
    pinMode(input, INPUT);
    if (trig != UNUSED_PIN) {
        pinMode(trig, OUTPUT);
        digitalWrite(trig, LOW);
    }
    // This device is an analog input, set request for reading
    a_values[idx] = A_REQUEST_FLAG;
}

/**
 * @brief Set up Devices. Port sense/ctrl and sensor input/trig pins
 */
void devices_setup() {
    VERBOSE("Devices setup", "");
    // Reset pin conflicts
    // TODO: Consider conflicts from utility.h, LED_PIN and DEBUG_LED_ENABLE
    for (uint8_t i = 0; i < NUM_P_DEVICES; i++) {
        p_device_setup(i);
    }

    for (uint8_t i = 0; i < NUM_A_DEVICES; i++) {
        a_device_setup(i);
    }

    last_tick_millis = millis();
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
        int poll_interval = get_int(POLL_INTERVAL);
        if (seconds % poll_interval == 0) {
            DEBUG("Poll ", seconds);
            for (uint8_t i = 0; i < NUM_A_DEVICES; i++) {
                update_a_device(i);
            }
        }
        for (uint8_t i = 0; i < NUM_P_DEVICES; i++) {
            update_p_device(i);
        }
        // Check for requested a device input
        for (uint8_t i = 0; i < NUM_A_DEVICES; i++) {
            if (a_values[i] == A_REQUEST_FLAG) {
                update_a_device(i);
            }
        }
        seconds += 1;
        while (current_millis - last_tick_millis > SEC) {
            last_tick_millis += SEC;
        }
    }
}
