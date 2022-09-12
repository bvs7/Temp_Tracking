#ifndef DEVICES_H_
#define DEVICES_H_

int dist_sensor(int trig_pin, int echo_pin);
void update_p_device(uint8_t idx);
void update_a_device(uint8_t idx);
void p_device_setup(uint8_t idx);
void a_device_setup(uint8_t idx);

void devices_setup();
void devices_loop();

#endif // DEVICES_H_

