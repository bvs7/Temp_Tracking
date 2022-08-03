#ifndef DEVICES_H_
#define DEVICES_H_

#include "utility.h"
#include "connection.h"
#include "commands.h"

class device{
  byte sense_pin;
  byte ctrl_pin;
  byte ctrl;
  state last_state = OFF_; // TODO get default state from EEPROM

  public:
  char device_name[SETTINGS_SIZE];

  device(byte sense_pin, byte ctrl_pin, const char* device_name);
  void setup();
  void loop();
  void set_ctrl(byte ctrl);
  bool check_name(const char* name, unsigned int length);
};

#endif