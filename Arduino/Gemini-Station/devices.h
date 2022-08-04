#ifndef DEVICES_H_
#define DEVICES_H_

#include "utility.h"
#include "connection.h"
#include "commands.h"


#define CTRL_MASK 0x01
#define SENSE_MASK 0x02
enum state{OFF_ = 0, UNPOWERED = 1, OVERRIDE = 2, ON_ = 3};


typedef struct device_settings{
  char device_name[SETTINGS_SIZE];
  byte sense_pin;
  byte ctrl_pin;
  state device_state;
} device_settings_t;

class device{

  device_settings s;
  byte ctrl;

  public:
  char device_name[SETTINGS_SIZE];

  device(byte sense_pin, byte ctrl_pin, const char* device_name);
  void setup();
  void loop();
  char *state_str();
  void set_ctrl(byte ctrl);
  bool check_name(const char* name, unsigned int length);
};


class sensor{
  public:
  sensor(byte sense_pin, const char* device_name);
  void setup();
  void loop();
};

#endif