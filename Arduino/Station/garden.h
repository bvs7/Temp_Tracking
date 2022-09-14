#include "settings.h"

#define FW_VERSION_ "0.0.1"
#define CATEGORY_ "garden"
#define STATION_NAME_ "beds-1"

// clang-format off
//                0  1  2  3           4
byte p_sense[] = {5, 7, 9, UNUSED_PIN, UNUSED_PIN};
byte p_ctrl[] =  {4, 6, 8, UNUSED_PIN, UNUSED_PIN};

//                0           1           2           3           4
byte a_input[] = {UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN};
byte a_trig[] =  {UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN, UNUSED_PIN};
// clang-format on

#define POLL_INTERVAL_ 300
