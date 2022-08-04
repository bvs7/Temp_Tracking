#ifndef COMMANDS_H_
#define COMMANDS_H_

//#define DEBUG_BAUD_RATE 9600
#define DEBUG_BAUD_RATE 115200

// #include "SerialCommands.h"
// #include "PipedStream.h" // Buffered Streams library for virtual stream for commands

void enter_command(char * command, unsigned int length);

void commands_setup();
void commands_loop();

// extern SerialCommands serial_commands_;

// extern PipedStream& command_handle;
// extern PipedStream& command_internal;
#endif
