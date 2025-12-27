#ifndef TIMEPROVIDER_H
#define TIMEPROVIDER_H

#include <Arduino.h>
#include <time.h>
#include "../config/config.h"

class TimeProvider {
public:
    void setup() {
        configTzTime(TIME_ZONE, NTP_SERVER);
    }

    String getFormattedTime() {
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            return "Syncing with NTP time ";
        }

        char timeStringBuff[30];
        strftime(timeStringBuff, sizeof(timeStringBuff), "%d-%m-%Y %I:%M:%S %p", &timeinfo);
        return String(timeStringBuff);
    }
};

extern TimeProvider time_provider;

#endif //TIMEPROVIDER_H
