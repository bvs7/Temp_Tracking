
#include "settings.h"

#define STATION_NAME "garden1"

// p_devices: sense, ctrl, name
#define P0_CONFIG {5,4, "bed0"}
#define P1_CONFIG {7,6, "bed1"}
#define P2_CONFIG {9,8, "cmpst2"}
#define P3_CONFIG {UNUSED_PIN, UNUSED_PIN, "P3"}
#define P4_CONFIG {UNUSED_PIN, UNUSED_PIN, "P4"}

#define POLL_INTERVAL 4

// a_devices: sense, trig, name
#define A0_CONFIG {UNUSED_PIN, UNUSED_PIN, "A0"}
#define A1_CONFIG {UNUSED_PIN, UNUSED_PIN, "A1"}
#define A2_CONFIG {UNUSED_PIN, UNUSED_PIN, "A2"}
#define A3_CONFIG {UNUSED_PIN, UNUSED_PIN, "A3"}
//#define A4_CONFIG {UNUSED_PIN, UNUSED_PIN, "A4"}
//#define A5_CONFIG {UNUSED_PIN, UNUSED_PIN, "A5"}
//#define A6_CONFIG {UNUSED_PIN, UNUSED_PIN, "A6"}
//#define A7_CONFIG {UNUSED_PIN, UNUSED_PIN, "A7"}

#define P0_STATE 0x00
#define P1_STATE 0x00
#define P2_STATE 0x00
#define P3_STATE 0x00
#define P4_STATE 0x00

#define A0_VALUE 0
#define A1_VALUE 0
#define A2_VALUE 0
#define A3_VALUE 0
//#define A4_VALUE 0
//#define A5_VALUE 0
//#define A6_VALUE 0
//#define A7_VALUE 0
