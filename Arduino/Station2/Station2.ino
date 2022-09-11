
#include "settings.h"

char name[SETTING_LEN] = {0};

void setup(){
    Serial.begin(9600);
    Serial.println("Starting");
    
    byte valid = GET_BYTE(VALID);
    if (valid != EEPROM_SETTINGS_VALID){
        Serial.println("Invalid settings");
        return; // Todo error handling
    }

    GET_STR(STATION_NAME, name);
    

}

void loop(){
}