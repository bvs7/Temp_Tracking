#include "utility.h"

#define MORSE_UNIT_MILLIS 70

char state_string[4][10] = {
  "OFF", "UNPOWERED", "OVERRIDE", "ON"
};

void dot(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(MORSE_UNIT_MILLIS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(MORSE_UNIT_MILLIS);
}

void dash(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(MORSE_UNIT_MILLIS * 3);
  digitalWrite(LED_BUILTIN, LOW);
  delay(MORSE_UNIT_MILLIS);
}

void space(){
  delay(MORSE_UNIT_MILLIS * 2);
}

void util_setup(){
  pinMode(LED_BUILTIN, OUTPUT);
}
