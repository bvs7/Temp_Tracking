#ifndef UTILITY_H_
#define UTILITY_H_

#ifndef DEBUG_LOG_LEVEL
#define DEBUG_LOG_LEVEL 7
#endif

#if DEBUG_LOG_LEVEL > 0
#define ERROR(x, y) if(DEBUG_LOG_LEVEL > 0) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define WARN(x, y) if(DEBUG_LOG_LEVEL > 1) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define INFO(x, y) if(DEBUG_LOG_LEVEL > 2) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define DEBUG(x, y) if(DEBUG_LOG_LEVEL > 3) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define ERROR2(x, y) if(DEBUG_LOG_LEVEL > 4) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define WARN2(x, y) if(DEBUG_LOG_LEVEL > 5) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define INFO2(x, y) if(DEBUG_LOG_LEVEL > 6) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#define DEBUG2(x, y) if(DEBUG_LOG_LEVEL > 7) {Serial.print(x); Serial.print(" "); Serial.println(y);}
#else
#define ERROR(x, y)
#define WARN(x, y)
#define INFO(x, y)
#define DEBUG(x, y)
#define ERROR2(x, y)
#define WARN2(x, y)
#define INFO2(x, y)
#define DEBUG2(x, y)
#endif


void dot();

void dash();

void space();

void util_setup();

#endif // UTILITY_H_
