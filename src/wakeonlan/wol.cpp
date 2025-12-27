#ifndef WOL_CPP
#define WOL_CPP

#include "wol.h"
#include "../connectivity/connectivity.h"
#include "../safety/safety.h"

extern Connectivity network;

Wol::Wol() {}

// TODO(ak) duplicated. Remove from Wol & WebService.
bool Wol::isThermalUnsafe() {
    float currentTemp = network.getInternalTemp();
    if (currentTemp > MAX_TEMP_C) {
        logger.log("[CRITICAL] : Temp " + String(currentTemp) + "C exceeds limit!", network.getFormattedTime());
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
            logger.log("[WoL] Wake Packet Received", network.getFormattedTime());
            if (!isThermalUnsafe() && onWakeUp) {
                // Wake Ubuntu (Default) with Default Strategy
                onWakeUp(OS_NAME_PRIMARY, DEFAULT_BOOT_STRATEGY);
            }
        }
    }
}

#endif
