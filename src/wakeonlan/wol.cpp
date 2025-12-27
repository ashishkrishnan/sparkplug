#ifndef WOL_CPP
#define WOL_CPP

#include "wol.h"

#include <utility>
#include "../system/systeminfo.h"
#include "../logger/EventLogger.h"
#include "../connectivity/connectivity.h"

extern Connectivity network;

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
    int packetSize = Udp.parsePacket();
    if (packetSize != 102) return;

    Udp.read(packetBuffer, 102);

    for (int i = 0; i < 6; i++) {
        if (packetBuffer[i] != 0xFF) return;
    }

    uint8_t receivedMacAddress[6];
    memcpy(receivedMacAddress, &packetBuffer[6], 6);

    uint8_t actualMacAddress[6];
    network.getMacBytes(actualMacAddress);

    uint8_t primaryMac[6];
    memcpy(primaryMac, actualMacAddress, 6);
    primaryMac[5] = VIRTUAL_MAC_HEX_PRIMARY;

    uint8_t secondaryMac[6];
    memcpy(secondaryMac, actualMacAddress, 6);
    secondaryMac[5] = VIRTUAL_MAC_HEX_SECONDARY;

    if (isThermalUnsafe()) return;

    if (memcmp(actualMacAddress, primaryMac, 6) == 0) {
        Log.log("[WoL] Waking up: " + String(OS_NAME_PRIMARY));
        if (onWakeUp) onWakeUp(OS_NAME_PRIMARY, DEFAULT_BOOT_STRATEGY);
    } else if (memcmp(actualMacAddress, secondaryMac, 6) == 0) {
        Log.log("[WoL] Waking up: " + String(OS_NAME_SECONDARY));
        if (onWakeUp) onWakeUp(OS_NAME_SECONDARY, DEFAULT_BOOT_STRATEGY);
    } else if (memcmp(receivedMacAddress, actualMacAddress, 6) == 0) {
        Log.log("[WoL] Waking up default OS: " + String(OS_NAME_PRIMARY));
        if (onWakeUp) onWakeUp(OS_NAME_PRIMARY, DEFAULT_BOOT_STRATEGY);
    }
}

#endif
