#ifndef COMMANDS_H_
#define COMMANDS_H_

void root_handle(char *input, Stream *resp);

void p_device_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp);
void a_device_handle(uint8_t idx, char *cmd, char **saveptr, Stream *resp);

#endif // DEVICE_H_