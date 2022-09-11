#include "Arduino.h"

#include "utility.h"
#include "settings.h"
#include "connection.h"

station_settings *d_sett;

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
    p_device_config *P = &d_sett->config.P[idx];
    if(P->sense == UNUSED_PIN){
        return;
    }
    device_state *state = &d_sett->state.P_state[idx];
    device_state new_state = 
        (digitalRead(P->sense) ? SENSE_MASK : 0) |
        (digitalRead(P->ctrl) ? CTRL_MASK : 0);
    if(*state != new_state){
        *state = new_state;
        publish_data(P->name, state_str[new_state], true);
    }
}

void update_a_device(uint8_t idx){
    a_device_config *A = &d_sett->config.A[idx];
    int *value = &d_sett->state.A_value[idx];
    int new_value = *value;
    if(A->input != UNUSED_PIN){
        if(A->trig == UNUSED_PIN){
            new_value = analogRead(A->input);
        } else {
            new_value = dist_sensor(A->trig, A->input);
        }
    }
    if(new_value != *value){
        *value = new_value;
        publish_data(A->name, String(*value).c_str(), true);
    }
}

uint8_t p_id_from_name(char *name){
    for(uint8_t i = 0; i < NUM_P_DEVICES; i++){
        if(strcmp(name, d_sett->config.P[i].name) == 0){
            return i;
        }
    }
    return 0xFF;
}

uint8_t a_id_from_name(char *name){
    for(uint8_t i = 0; i < NUM_A_DEVICES; i++){
        if(strcmp(name, d_sett->config.A[i].name) == 0){
            return i;
        }
    }
    return 0xFF;
}


void p_device_setup(p_device_config *p, device_state s){
    if(p->sense == UNUSED_PIN){
        return;
    }
    // TODO: check pin conflicts
    pinMode(p->sense, INPUT_PULLUP);
    pinMode(p->ctrl, OUTPUT);
    digitalWrite(p->ctrl, s & CTRL_MASK);
}

void a_device_setup(a_device_config *a, int *value){
    if(a->input == UNUSED_PIN){
        return;
    }
    // TODO: check pin conflicts
    pinMode(a->input, INPUT);
    if(a->trig != UNUSED_PIN){
        pinMode(a->trig, OUTPUT);
        digitalWrite(a->trig, LOW);
    }
    // This device is an analog input, set request for reading
    *value = A_REQUEST_FLAG;
}


void devices_setup(station_settings *s){
    d_sett = s;

    for(uint8_t i = 0; i < NUM_P_DEVICES; i++){
        p_device_setup(&d_sett->config.P[i], d_sett->state.P_state[i]);
    }

    for(uint8_t i = 0; i < NUM_A_DEVICES; i++){
        a_device_setup(&d_sett->config.A[i], &d_sett->state.A_value[i]);
    }

    last_tick_millis = millis();
}

void devices_loop(){
    // TODO: a 1 second loop and a a_device loop every n seconds
    unsigned long current_millis = millis();
    if(current_millis - last_tick_millis > SEC){
        seconds += 1;
        Serial.println("tick");
        if(seconds % d_sett->config.a_device_poll_interval == 0){
            for(uint8_t i = 0; i < NUM_A_DEVICES; i++){
                update_a_device(i);
            }
        }else{
            for(uint8_t i = 0; i < NUM_P_DEVICES; i++){
                update_p_device(i);
            }
            // Check for requested a device input
            for(uint8_t i = 0; i < NUM_A_DEVICES; i++){
                if(d_sett->state.A_value[i] == A_REQUEST_FLAG){
                    update_a_device(i);
                }
            }
        }
         while(current_millis - last_tick_millis > SEC){
            last_tick_millis += SEC;
        }
    }
}
