#include "Arduino.h"

#include "utility.h"
#include "settings.h"
#include "connection.h"

unsigned long last_tick_millis = 0;
unsigned long seconds = 0;

int dist_sensor(int trig_pin, int echo_pin){
    // Make sure that trigger pin is LOW.
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance.
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    // Measure time it takes for echo to go HIGH and then LOW.
    unsigned long maxDistanceDurationMicroSec = 53000; // 3 meters
    
    unsigned long durationMicroSec = pulseIn(echo_pin, HIGH, maxDistanceDurationMicroSec);

    // Calculate distance in millimeters.
    return (int) (durationMicroSec / 2 * 0.0343);
}

void update_p_device(uint8_t idx){
    byte sense = get_byte(P_SENSE(idx));
    if(sense == UNUSED_PIN){
        return;
    }
    byte ctrl = get_byte(P_CTRL(idx));
    p_state new_state = 
        (digitalRead(sense) ? SENSE_MASK : 0) |
        (digitalRead(ctrl) ? CTRL_MASK : 0);
    if(p_states[idx] != new_state){
        WARN("State update! for P", idx);
        WARN("", state_str[new_state]);
        p_states[idx] = new_state;
        char topic[3] = "P0";
        topic[1] += idx;
        publish_data(topic, state_str[new_state], true);
    }
}

void update_a_device(uint8_t idx){
    byte input = get_byte(A_INPUT(idx));
    if(input == UNUSED_PIN){
        return;
    }
    int new_value = a_values[idx];
    byte trig = get_byte(A_TRIG(idx));
    if(trig == UNUSED_PIN){
        new_value = analogRead(input);
    } else {
        new_value = dist_sensor(trig, input);
    }
    if(new_value != a_values[idx]){
        a_values[idx] = new_value;
        char topic[3] = "A0";
        topic[1] += idx;
        char value_str[6];
        publish_data(topic, itoa(new_value, value_str, 10), true);
    }
}

void p_device_setup(uint8_t idx){
    byte sense = get_byte(P_SENSE(idx));
    if(sense == UNUSED_PIN){
        return;
    }
    byte ctrl = get_byte(P_CTRL(idx));
    // TODO: check pin conflicts
    pinMode(sense, INPUT);
    pinMode(ctrl, OUTPUT);
    digitalWrite(ctrl, get_byte(p_states[idx]) & CTRL_MASK);
}

void a_device_setup(uint8_t idx){
    byte input = get_byte(A_INPUT(idx));
    if(input == UNUSED_PIN){
        return;
    }
    byte trig = get_byte(A_TRIG(idx));
    // TODO: check pin conflicts
    pinMode(input, INPUT);
    if(trig != UNUSED_PIN){
        pinMode(trig, OUTPUT);
        digitalWrite(trig, LOW);
    }
    // This device is an analog input, set request for reading
    a_values[idx] = A_REQUEST_FLAG;
}


void devices_setup(){
    DEBUG2("Devices setup", "");
    // Reset pin conflicts
    for(uint8_t i = 0; i < NUM_P_DEVICES; i++){
        p_device_setup(i);
    }

    for(uint8_t i = 0; i < NUM_A_DEVICES; i++){
        a_device_setup(i);
    }

    last_tick_millis = millis();
}

void devices_loop(){
    DEBUG2("Devices loop", "");
    unsigned long current_millis = millis();
    if(current_millis - last_tick_millis > SEC){
        seconds += 1;
        INFO("Tick","");
        dot();
        int a_poll_interval = get_int(A_POLL_INTERVAL);
        if(seconds % a_poll_interval == 0){
            for(uint8_t i = 0; i < NUM_A_DEVICES; i++){
                update_a_device(i);
            }
        }
        for(uint8_t i = 0; i < NUM_P_DEVICES; i++){
            update_p_device(i);
        }
        // Check for requested a device input
        for(uint8_t i = 0; i < NUM_A_DEVICES; i++){
            if(a_values[i] == A_REQUEST_FLAG){
                update_a_device(i);
            }
        }
         while(current_millis - last_tick_millis > SEC){
            last_tick_millis += SEC;
        }
    }
}
