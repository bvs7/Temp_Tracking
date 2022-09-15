// File to pull settings from EEPROM
#include "settings.h"

#include "Arduino.h"
#include "EEPROM.h"

// EEPROM length is 1024 bytes, 0x400

#define START_ADDR 0x020

/** @brief Get the byte stored at addr */
byte get_byte(int addr) { return EEPROM.read(addr); }

/** @brief Get the int stored at addr/addr+1 */
int get_int(int addr) {
    return (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
}

/**
 * @brief Get the str object at addr of length
 *
 * @return char* a pointer to an allocated section of memory
 */
char *get_str(int addr, size_t length) {
    char *s = (char *)malloc(length);
    memset(s, 0, length);
    for (uint8_t i = 0; i < length - 1; i++) {
        s[i] = EEPROM.read(addr + i);
        if (s[i] == '\0') {
            break;
        }
    }
    return s;
}

/** @brief Set the byte at addr of EEPROM */
void set_byte(int addr, byte val) { EEPROM.update(addr, val); }

/** @brief Set the int at addr/addr+1 of EEPROM */
void set_int(int addr, int val) {
    EEPROM.update(addr, val >> 8);
    EEPROM.update(addr + 1, val & 0xFF);
}

/** @brief Write str s to EEPROM, max length */
void set_str(int addr, char *s, size_t length) {
    for (uint8_t i = 0; i < length - 1; i++) {
        EEPROM.update(addr + i, s[i]);
        if (s[i] == '\0') {
            break;
        }
    }
}