
#include "commands.h"

#include "utility.h"

#define BUFFER_SIZE 64
// Create command buffer

extern char* get_setting(const char* var);
extern bool set_setting(const char* var, const char* value);
extern void mqtt_callback(char* topic, byte* payload, unsigned int length);

char serial_command_buffer_[BUFFER_SIZE];
byte buffer_pos_ = 0;
char* saveptr;

char* next(){
    delay(10);
    return strtok_r(NULL, " ", &saveptr);
}

void cmd_echo(){
    for(char* arg = next(); arg != NULL; arg = next()){
        Serial.print(arg);
        Serial.print(" ");
    }
    Serial.println("!");
}

void cmd_get(){
    char* var = next();
    if(var == NULL){
        LOG("No variable specified!\n");
        return;
    }
    char* value = get_setting(var);
    if(value != NULL){
        Serial.print(var);
        Serial.print(": ");
        Serial.println(value);
        return;
    }else{
        LOG("No value found for variable: ");
        LOG(var);
        LOG("\n");
    }
}

void cmd_set(){
    char* var = next();
    if(var == NULL){
        LOG("No variable specified!\n");
        return;
    }
    char* value = next();
    if(value == NULL){
        LOG("No value specified!\n");
        return;
    }
    set_setting(var, value);
}

void cmd_test_mqtt(){
    char* topic = next();
    if(topic == NULL){
        LOG("No topic specified!\n");
        return;
    }
    char* payload = saveptr;
    if(payload == NULL){
        payload = "";
    }
    unsigned int payload_length = strlen(payload);
    mqtt_callback(topic, (byte*)payload, payload_length);
}

void process_command(){
    // Check serial_command_buffer_ for command using strtok

    char* cmd = strtok_r(serial_command_buffer_, " ", &saveptr);
    if(cmd == NULL){
        LOG("No command found\n");
        return;
    }
    
    if(strcmp(cmd, "echo") == 0){
        cmd_echo();
    }else if(strcmp(cmd, "get") == 0){
        cmd_get();
    }else if(strcmp(cmd, "set") == 0){
        cmd_set();
    }else if(strcmp(cmd, "test_mqtt") == 0){
        cmd_test_mqtt();
    }else{
      LOG("Unknown command: ");
      LOG(cmd);
      LOG("\n");
    }

}

void enter_command(char * command, unsigned int length){
    if(length >= BUFFER_SIZE){
        LOG("Command too long!\n");
        return;
    }
    strcpy(serial_command_buffer_, command);
    process_command();
}

void commands_setup() {
    Serial.begin(DEBUG_BAUD_RATE);
    delay(100);
    
}

void commands_loop() {
  while(Serial.available()>0){
    char ch = Serial.read();
    if(ch == '\n'  || ch == '\r'|| ch == '\0'){
        serial_command_buffer_[buffer_pos_] = '\0';
        process_command();
        buffer_pos_ = 0;
        break;
    }
    else{
        if (buffer_pos_ < BUFFER_SIZE) {
            serial_command_buffer_[buffer_pos_++] = ch;
        }else{
            // Overflow! just stop writing...
        }
    }
  }

  Serial.flush();
  delay(10);
}
