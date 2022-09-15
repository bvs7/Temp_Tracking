#include "utility.h"

#include "Arduino.h"

#define DEBUG_LED_ENABLE
#ifdef DEBUG_LED_ENABLE

#define LED_PIN LED_BUILTIN

#define MORSE_UNIT_MILLIS 100

/** @brief Blink LED for 1 unit, then pause for 1 unit */
void dot() {
    digitalWrite(LED_PIN, HIGH);
    delay(MORSE_UNIT_MILLIS);
    digitalWrite(LED_PIN, LOW);
    delay(MORSE_UNIT_MILLIS);
}

/** @brief Blink LED for 3 units, then pause for 1 unit */
void dash() {
    digitalWrite(LED_PIN, HIGH);
    delay(MORSE_UNIT_MILLIS * 3);
    digitalWrite(LED_PIN, LOW);
    delay(MORSE_UNIT_MILLIS);
}

/** @brief Pause for 2 units */
void space() { delay(MORSE_UNIT_MILLIS * 2); }

/** @brief Set up LED pin */
void util_setup() { pinMode(LED_PIN, OUTPUT); }

#endif
