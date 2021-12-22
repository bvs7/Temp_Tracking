#include "utility.h"

void dot(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}

void dash(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(750);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}

void space(){
  delay(500);
}

void util_setup(){
  pinMode(LED_BUILTIN, OUTPUT);
}
