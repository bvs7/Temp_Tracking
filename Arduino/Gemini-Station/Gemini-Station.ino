/*
 * gemini-home automation Arduino MQTT project
*/
#include "utility.h"
#include "SerialCommands.h"
#include "connection.h"
#include "commands.h"

void setup()
{
  util_setup();
  commands_setup();
  connection_setup();
}

void loop()
{
  connection_loop();
  commands_loop();
}
