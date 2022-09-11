// File to pull settings from EEPROM
#include "Arduino.h"

#include "settings.h"

#include "EEPROM.h"

// EEPROM length is 1024 bytes, 0x400

#define START_ADDR 0x020

void * get_eeprom(int offset,  int size){
    void *ptr = malloc(size);
    for(int i = 0; i < size; i++){
        *((char *) ptr + i) = EEPROM.read(START_ADDR + offset + i);
    }
    return ptr;
}
