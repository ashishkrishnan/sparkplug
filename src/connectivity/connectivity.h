#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H
#include <Arduino.h>

class IConnection {
    public:
        virtual ~IConnection() = default;

        virtual void setupConnectivity() = 0;
        virtual void handleConnectivityLoop() = 0;
        virtual bool isTargetPCAlive(const char* targetIp) = 0;
        virtual String getIpAddress() = 0;
        virtual long getWifiSignalStrength() = 0;
        virtual float getInternalTemp() = 0;
};

#endif //CONNECTIVITY_H
