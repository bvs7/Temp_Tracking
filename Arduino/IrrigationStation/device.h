#ifndef DEVICE_H_
#define DEVICE_H_

#include "settings.h"

class device {
public:

    uint8_t sense_pin, ctrl_pin;
    device_settings *settings;

    device(uint8_t sense_pin, uint8_t ctrl_pin, device_settings* settings);

    bool loop_1s();
    void set_ctrl(uint8_t state);

};

#endif // DEVICE_H_