#ifndef WEBAPI_H
#define WEBAPI_H
#include <Arduino.h>

class WebApi {
    public:
        typedef void (*WakeCallback)(String os);
        typedef void (*ShutDownCallback)();

        void setupWebApi(WakeCallback onWake, ShutDownCallback onShutDown);
        void handleWebApiLoop();

        virtual void logEvent(String msg);
};
#endif //WEBAPI_H
