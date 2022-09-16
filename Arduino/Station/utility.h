#ifndef UTILITY_H_
#define UTILITY_H_

#define DEBUG_LOG_LEVEL 2

/* Debug levels:
0 - No debug
1 - Errors
2 - Warnings
3 - Info
4 - Debug
5 - Verbose
6 - Very Verbose
*/

// clang-format off
#if DEBUG_LOG_LEVEL > 0
#define ERROR(x, y) if(DEBUG_LOG_LEVEL >= 1) {Serial.print(x); Serial.println(y);}
#define ERR(f, n) if(DEBUG_LOG_LEVEL >= 1) {Serial.print("ERROR: "); Serial.print(f); Serial.println(n);}
#define WARN(x, y) if(DEBUG_LOG_LEVEL >= 2) {Serial.print(x); Serial.println(y);}
#define INFO(x, y) if(DEBUG_LOG_LEVEL >= 3) {Serial.print(x); Serial.println(y);}
#define DEBUG(x, y) if(DEBUG_LOG_LEVEL >= 4) {Serial.print(x); Serial.println(y);}
#define VERBOSE(x, y) if(DEBUG_LOG_LEVEL >= 5) {Serial.print(x); Serial.println(y); Serial.flush(); delay(50);}
#define VERBOSE2(x, y) if(DEBUG_LOG_LEVEL >= 6) {Serial.print(x); Serial.println(y); Serial.flush(); delay(50);}
#else
#define ERROR(x, y)
#define ERR(f, n)
#define WARN(x, y)
#define INFO(x, y)
#define DEBUG(x, y)
#define VERBOSE(x, y)
#define VERBOSE2(x, y)
#endif
// clang-format on

void dot();

void dash();

void space();

void util_setup();

#endif  // UTILITY_H_
