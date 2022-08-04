#ifndef UTILITY_H_
#define UTILITY_H_

#define DEBUG_LOG_LEVEL 0

#if DEBUG_LOG_LEVEL > 0
#define ERROR(x, y) if(DEBUG_LOG_LEVEL > 0) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define WARN(x, y) if(DEBUG_LOG_LEVEL > 1) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define INFO(x, y) if(DEBUG_LOG_LEVEL > 2) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define DEBUG(x, y) if(DEBUG_LOG_LEVEL > 3) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#else
#define ERROR(x, y)
#define WARN(x, y)
#define INFO(x, y)
#define DEBUG(x, y)
#endif


void dot();

void dash();

void space();

void util_setup();

#endif // UTILITY_H_
