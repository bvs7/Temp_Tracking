// File to pull settings from EEPROM
#include "settings.h"

#include "Arduino.h"
#include "EEPROM.h"


/**
 * @brief Convert pin string to Arduino pin number
 *
 * @param pin pin name e.g. "D0", "A3", "12", "-"
 * @return uint8_t pin number e.g. 0, 3, 12, UNUSED_PIN
 */
uint8_t pin_name_to_num(char *pin) {
    if (pin[0] == 'A') {
        return atoi(pin + 1) + 14;
    } else if (pin[0] == '-') {
        return UNUSED_PIN;
    } else {
        return atoi(pin);
    }
}

/**
 * @brief Convert Arduino pin number to pin string
 *
 * @param pin pin number e.g. 0, 3, 12, UNUSED_PIN
 * @param name pin name e.g. "D0", "A3", "D12", "-"
 */
void pin_num_to_name(uint8_t pin, char *name) {
    if (pin == UNUSED_PIN) {
        sprintf(name, "-");
    } else if (pin > 13) {
        sprintf(name, "A%d", pin - 14);
    } else {
        sprintf(name, "D%d", pin);
    }
}

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
    if(s == NULL) {
        Serial.println("get_str fail!");
        return NULL;
    }
    memset(s, 0, length);
    for (uint8_t i = 0; i < length - 1; i++) {
        s[i] = EEPROM.read(addr + i);
        if (s[i] == '\0') {
            break;
        }
    }
    return s;
}

void get_str_(int addr, size_t length, char *s) {
    memset(s, 0, length);
    for (uint8_t i = 0; i < length - 1; i++) {
        s[i] = EEPROM.read(addr + i);
        if (s[i] == '\0') {
            break;
        }
    }
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

bool check_fw_version(){
    char fw_version[SETTING_LEN];
    get_str_(FW_VERSION, SETTING_LEN, fw_version);
    if (strcmp(fw_version, FW_VERSION_) != 0) {
        Serial.print("EEPROM FW version mismatch: ");
        Serial.print(fw_version);
        Serial.print(" != ");
        Serial.println(FW_VERSION_);
        return false;
    }
    return true;
}