#ifndef STATION_H_
#define STATION_H_

#include "utility.h"
#include "connection.h"
#include "devices.h"

#define NUM_DEVICES 5
#define NUM_SENSORS 8

// Make a main class with a setup and loop function that has 4 device instances P0-4
class station{

    public:
    connection conn;
    device P[NUM_DEVICES];
    sensor A[NUM_SENSORS];

    station(const char* station_name);

    void save();
    void load();

    void setup();
    void loop();

};

#endif

