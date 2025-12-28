#ifndef WOL_CPP
#define WOL_CPP

#include "wol.h"

#include <utility>
#include "../system/systeminfo.h"
#include "../logger/EventLogger.h"
#include "../connectivity/connectivity.h"

extern Connectivity network;

Wol::Wol() {
    memset(_systemMac, 0, 6);
}

void Wol::setupWol(WakeCallback onWakeUpCallback) {
    onWakeUp = onWakeUpCallback;
    Udp.begin(WOL_PORT);
    network.getMacBytes(_systemMac);
    Log.log("[WoL] Setup complete");
}

void Wol::handleWolLoop() {
    int packetSize = Udp.parsePacket();
    if (packetSize < 102) return;

    Udp.read(packetBuffer, 102);

    for (int i = 0; i < 6; i++) {
        if (packetBuffer[i] != 0xFF) return;
    }

    uint8_t receivedMac[6];
    memcpy(receivedMac, &packetBuffer[6], 6);

    uint8_t actualMac[6];
    memcpy(actualMac, _systemMac, 6);

    uint8_t primaryMac[6];
    memcpy(primaryMac, actualMac, 6);
    primaryMac[5] = VIRTUAL_MAC_HEX_PRIMARY;

    uint8_t secondaryMac[6];
    memcpy(secondaryMac, actualMac, 6);
    secondaryMac[5] = VIRTUAL_MAC_HEX_SECONDARY;

    if (memcmp(receivedMac, primaryMac, 6) == 0) {
        if (onWakeUp) onWakeUp(OS_NAME_PRIMARY, DEFAULT_BOOT_STRATEGY);
    } else if (memcmp(receivedMac, secondaryMac, 6) == 0) {
        if (onWakeUp) onWakeUp(OS_NAME_SECONDARY, DEFAULT_BOOT_STRATEGY);
    } else if (memcmp(receivedMac, actualMac, 6) == 0) {
        if (onWakeUp) onWakeUp(OS_NAME_PRIMARY, DEFAULT_BOOT_STRATEGY);
    }
}

#endif
