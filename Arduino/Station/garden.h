
#include "settings.h"

#define FW_VERSION_ "0.0.0"
#define STATION_NAME_ "garden1"

byte p_sense[] = {5, 7, 9, UNUSED_PIN, UNUSED_PIN};
byte p_ctrl[] = {4, 6, 8, UNUSED_PIN, UNUSED_PIN};

byte a_input[] = {UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN};
byte a_trig[] = {UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN};

#define A_POLL_INTERVAL_ 300
