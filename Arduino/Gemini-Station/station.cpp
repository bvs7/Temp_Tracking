
#include "station.h"

// Create boilerplate code for functions of the station class
station::station(const char* station_name){
    conn = connection(station_name);
    P[0] = device(P0_SENSE_PIN, P0_CTRL_PIN, "P0");
}
}

station::setup(){
    // How to set up station settings?
    // Pump subclass? Valve subclass?
    // Names for devices.


}
