#ifndef UTILITY_H_
#define UTILITY_H_

#include "Arduino.h"

void dot();
void dash();
void space();
void util_setup();

#define SETTINGS_SIZE 32

typedef struct connection_settings{
  char ssid[SETTINGS_SIZE];
  char pass[SETTINGS_SIZE];
  char server[SETTINGS_SIZE];
  char mqtt_id[SETTINGS_SIZE];
} connection_settings_t;

extern connection_settings_t conn_sett;

enum state{OFF, UNPOWERED, OVERRIDE, ON};

class sense_ctrl_port{
  unsigned char sense;
  unsigned char ctrl;
  unsigned char ctrl_state = 0;
  
  public:
  sense_ctrl_port(unsigned char sense_pin, unsigned char ctrl_pin){
    sense = sense_pin;
    ctrl = ctrl_pin;
  
    pinMode(sense, INPUT);
    pinMode(ctrl, OUTPUT);

    digitalWrite(ctrl, ctrl_state);
  }

  unsigned char get_state(){
    return (digitalRead(sense) << 1) | ctrl_state;
  }

  void set_output(unsigned char ctrl_output){
    ctrl_state = ctrl_output;
    digitalWrite(ctrl, ctrl_output);
  }
};

#endif
