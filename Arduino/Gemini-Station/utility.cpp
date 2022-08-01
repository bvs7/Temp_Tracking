#include "utility.h"

connection_settings_t conn_sett;

#define MORSE_UNIT_MILLIS 70

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
