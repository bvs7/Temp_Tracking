
//#define PROGRAMMING_MODE

#ifdef PROGRAMMING_MODE
#include "programming.h"

void setup(){
    programming_setup();
    Serial.begin(115200);
    Serial.println("Programming complete");
}
void loop(){
    dot(); space();
    Serial.print(".");
}

#else

#include "utility.h"
#include "devices.h"
#include "commands.h"
#include "connection.h"

#include "LoopbackStream.h"
#include "Adafruit_SleepyDog.h"

station_settings settings;

#define BUFFER_SIZE 16

LoopbackStream SerialIn(BUFFER_SIZE);
LoopbackStream MQTTOut(BUFFER_SIZE);


void mqtt_station_command_callback(byte *payload, unsigned int length){
    char *input = (char *) payload;
    input[length] = '\0';
    root_handle(input, &MQTTOut);
    publish_log(MQTTOut.readString().c_str(), false);
}

void mqtt_callback(char *topic, byte *payload, unsigned int length) {
    // parse topic
    char *saveptr;
    char *mode = strtok_r(topic, "/", &saveptr);
    if(strcmp(mode, CMD) != 0){
        return;
    }
    char *irrigation = strtok_r(NULL, "/", &saveptr);
    if(strcmp(irrigation, IRRIGATION) != 0){
        return;
    }
    char *station_name = strtok_r(NULL, "/", &saveptr);
    if(strcmp(station_name, settings.config.connection.station_name) != 0){
        return;
    }
    char *device_name = strtok_r(NULL, "/", &saveptr);
    if(device_name == NULL){
        // Station command
        mqtt_station_command_callback(payload, length);
        return;
    }else{
        char *saveptr = NULL;
        char input_buffer[64];
        snprintf(input_buffer, 64, "%s %s", device_name, payload);
        char *cmd = strtok_r(input_buffer, " ", &saveptr);
        uint8_t p_id = p_id_from_name(device_name);
        if(p_id != 0xFF){
            p_device_state_handle(p_id, saveptr, &saveptr, &MQTTOut);
            publish_log(MQTTOut.readString().c_str(), false);
            return;
        }
        uint8_t a_id = a_id_from_name(device_name);
        if(a_id != 0xFF){
            a_device_state_handle(a_id, saveptr, &saveptr, &MQTTOut);
            publish_log(MQTTOut.readString().c_str(), false);
            return;
        }
    }
}

void serial_callback(int len){
    Serial.println(len);
    char input[BUFFER_SIZE] = {0};
    SerialIn.readBytes(input, len);
    Serial.flush();
    root_handle(input, &Serial);
}

void setup() {
    Watchdog.disable();
    Serial.begin(9600);
    // put your setup code here, to run once:
    util_setup();
    if(!(load_settings(&settings))){
        // Error?
        // restart in 5 seconds
        Watchdog.enable(5000);
        while(1){
          dash(); space(); space();
        }
    }
    dot(); space();
    //connection_setup(&settings.config.connection);

    char topic_buf[64];
    snprintf(topic_buf, 64, "%s/%s/%s/#", 
        CMD, IRRIGATION, settings.config.connection.station_name);
    subscribe(topic_buf, 0);
    set_callback(mqtt_callback);
    dot(); dot(); space();
    devices_setup(&settings);
    commands_setup(&settings);
}

void loop() {
    // put your main code here, to run repeatedly:
    //connection_loop();
    devices_loop();
    static int len = 0;
    int c = Serial.read();
    while(c != -1){
        Serial.write(c);
        if(c == '\n'){
            serial_callback(len);
            len = 0;
        }else{
            len += 1;
            SerialIn.write(c);
        }
        c = Serial.read();
    }
}

#endif
