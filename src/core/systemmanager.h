#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "../power/power.h"
#include "../boot/boot.h"
#include "../safety/safety.h"
#include "../logger/EventLogger.h"

enum class CommandResult {
    SUCCESS,
    BUSY,
    COOLING_DOWN,
    THERMAL_UNSAFE,
    ALREADY_ONLINE,
    ALREADY_OFFLINE
};

class SystemManager {
private:
    Power *_power;
    Boot *_boot;
    Safety *_safety;

public:
    void setup(Power *power, Boot *boot, Safety *safety) {
        _power = power;
        _boot = boot;
        _safety = safety;
    }

    CommandResult triggerWake(String os, String strategy, bool force = false, String source = "Unknown") {
        Log.log("[Manager] Wake Request from " + source);

        if (!_safety->isThermalSafe()) {
            return CommandResult::THERMAL_UNSAFE;
        }

        if (_boot->isBusy()) {
            return CommandResult::BUSY;
        }

        if (!force && _boot->isCoolingDown()) {
            Log.log("[Manager] Wake Rejected: Cooling down in progress (" + String(_boot->getCoolDownRemaining()) + "s)");
            return CommandResult::COOLING_DOWN;
        }

        if (_safety->isTargetOnline()) {
            Log.log("[Manager] Wake Skipped: Target Online");
            return CommandResult::ALREADY_ONLINE;
        }

        Log.log("[Manager] Target PC is offline. Executing Wake for " + os);
        _power->triggerPulse();
        _boot->startSequence(os, strategy);

        return CommandResult::SUCCESS;
    }

    CommandResult triggerShutdown(String source = "Unknown") {
        Log.log("[Manager] Shutdown Request: " + source);

        if (!_safety->isThermalSafe()) {
            return CommandResult::THERMAL_UNSAFE;
        }

        if (_boot->isBusy()) {
            return CommandResult::BUSY;
        }

        if (!_safety->isTargetOnline()) {
            Log.log("[Manager] Shutdown Skipped: Target Offline");
            return CommandResult::ALREADY_OFFLINE;
        }

        if (_boot->isCoolingDown()) {
            Log.log("[Manager] Shutdown Rejected: Cooling down in progress (" + String(_boot->getCoolDownRemaining()) + "s)");
            return CommandResult::COOLING_DOWN;
        }

        Log.log("[Manager] Target PC is online. Executing Shutdown");
        _power->triggerPulse();
        _boot->startShutdown();

        return CommandResult::SUCCESS;
    }

    long getCoolDownRemaining() {
        return _boot->getCoolDownRemaining();
    }

    void update() {
        if (_boot) _boot->update();
    }
};

extern SystemManager system_manager;

#endif
