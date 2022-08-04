
#include "device.h"

device::device(uint8_t sense_pin, uint8_t ctrl_pin, device_settings* settings):
    sense_pin(sense_pin), ctrl_pin(ctrl_pin), settings(settings) {
    pinMode(sense_pin, INPUT);
    pinMode(ctrl_pin, OUTPUT);
    digitalWrite(ctrl_pin, settings->state & CTRL_MASK);
}

bool device::loop_1s(){
    if(digitalRead(ctrl_pin) != (settings->state & CTRL_MASK)){
        digitalWrite(ctrl_pin, settings->state & CTRL_MASK);
        return false;
    }
    uint8_t new_state = digitalRead(sense_pin)<<1 | digitalRead(ctrl_pin);
    if(new_state != settings->state){
        settings->state = new_state;
        return true;
    }
    return false;
}

void device::set_ctrl(uint8_t state){
    settings->state = (settings->state & SENSE_MASK) | state ;
}
