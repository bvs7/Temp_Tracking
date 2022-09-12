// File to pull settings from EEPROM
#include "Arduino.h"

#include "settings.h"

#include "EEPROM.h"

// EEPROM length is 1024 bytes, 0x400

#define START_ADDR 0x020

byte get_byte(int addr){
    return EEPROM.read(addr);
}

int get_int(int addr){
    return (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
}

char *get_str(int addr, size_t length){
    char *s = (char *)malloc(length);
    for(uint8_t i = 0; i < length; i++){
        s[i] = EEPROM.read(addr + i);
        if(s[i] == '\0'){
            break;
        }
    }
    return s;
}

void set_byte(int addr, byte val){
    EEPROM.update(addr, val);
}

void set_int(int addr, int val){
    EEPROM.update(addr, val >> 8);
    EEPROM.update(addr + 1, val & 0xFF);
}

void set_str(int addr, char *s, size_t length){
    for(uint8_t i = 0; i < length; i++){
        EEPROM.update(addr + i, s[i]);
        if (s[i] == '\0'){
            break;
        }
    }
}