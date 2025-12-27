#ifndef WOL_CPP
#define WOL_CPP

#include "wol.h"

#include <utility>
#include "../safety/safety.h"
#include "../system/systeminfo.h"
#include "../logger/EventLogger.h"

Wol::Wol() {}

// TODO(ak) duplicated. Remove from Wol & WebService.
bool Wol::isThermalUnsafe() {
    float currentTemp = system_info.getInternalTemp();
    if (currentTemp > MAX_TEMP_C) {
        Log.log("[CRITICAL] : Temp " + String(currentTemp) + "C exceeds limit!");
        return true;
    }

    return false;
}

void Wol::setupWol(WakeCallback onWakeUpCallback) {
    onWakeUp = onWakeUpCallback;
    Udp.begin(WOL_PORT);
}

void Wol::handleWolLoop() {
    int size = Udp.parsePacket();
    if (size == 102) {
        Udp.read(packetBuffer, 102);
        if (packetBuffer[0] == 0xFF) {
            Log.log("[WoL] Wake Packet Received");
            if (!isThermalUnsafe() && onWakeUp) {
                // Wake Ubuntu (Default) with Default Strategy
                onWakeUp(OS_NAME_PRIMARY, DEFAULT_BOOT_STRATEGY);
            }
        }
    }
}

#endif
