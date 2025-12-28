#ifndef SAFETY_H
#define SAFETY_H

#include "../../src/system/SystemInfo.h"
#include "../../src/connectivity/connectivity.h"
#include "../../src/logger/EventLogger.h"
#include "../../src/config/config.h"

class Safety {
private:
    Connectivity *_network;

public:
    Safety(Connectivity *network)
        : _network(network) {
    }

    void setup(Connectivity *network) {
        _network = network;
    }

    bool isThermalSafe() {
        float currentTemp = system_info.getInternalTemp();

        if (currentTemp > MAX_TEMP_C) {
            Log.log(
                "[Safety] CRITICAL: Temp " + String(currentTemp, 1) + "C exceeds limit (" + String(MAX_TEMP_C) + "C)");
            return false;
        }
        return true;
    }

    bool isSafeToWake(bool force) {
        if (!isThermalSafe()) return false;

        if (_network && !force) {
            if (_network->isTargetPCAlive()) {
                Log.log("[Safety] Blocked: Target PC is already ONLINE. Use force=true to override if available");
                return false;
            }
        }

        return true;
    }

    bool isSafeToShutdown(bool force) {
        if (!isThermalSafe()) return false;

        if (_network && !force) {
            if (!_network->isTargetPCAlive()) {
                Log.log("[Safety] Blocked: Target PC is already OFFLINE.");
                return false;
            }
        }

        return true;
    }

    bool isTargetOnline() {
        return _network->isTargetPCAlive();
    }
};

#endif
