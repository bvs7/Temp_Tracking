#include "settings.h"

#define CATEGORY_ "gemini-home"
#define STATION_NAME_ "tank-0"

// clang-format off
byte p_sense[] = {5, UNUSED_PIN, 9, UNUSED_PIN, UNUSED_PIN};
byte p_ctrl[] =  {4, UNUSED_PIN, 8, UNUSED_PIN, UNUSED_PIN};

byte a_input[] = {UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN};
byte a_trig[] =  {UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN};
// clang-format on

#define POLL_INTERVAL_ 300
