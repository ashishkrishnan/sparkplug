#ifndef BOOT_CPP_H
#define BOOT_CPP_H

#include "boot.h"
#include "../config/config.h"
#include "usbkeyboard.h"

Boot::Boot(IKeyboard *kb, Logger logger) : _kb(kb), _logger(std::move(logger)) {
    _isBusy = false;
    _lastSequenceFinishTime = 0;
}

bool Boot::isBusy() {
    return _isBusy;
}

bool Boot::isCoolingDown() {
    if (_isBusy) return false;
    unsigned long elapsed = (millis() - _lastSequenceFinishTime) / 1000;
    return (elapsed < POST_BOOT_COOL_DOWN_IN_SECONDS);
}

long Boot::getCoolDownRemaining() {
    if (!isCoolingDown()) return 0;
    unsigned long elapsed = (millis() - _lastSequenceFinishTime) / 1000;
    return POST_BOOT_COOL_DOWN_IN_SECONDS - elapsed;
}

void Boot::selectOS(const String &os, const String &strategy) {
    if (_isBusy) return;
    _isBusy = true;

    _logger("[BOOT] Sequence Started for " + os);

    if (strategy == "aggressive") {
        performAggressiveWait();
    } else {
        performStandardWait();
    }

    performNavigation(os);

    _isBusy = false;
    _lastSequenceFinishTime = millis();
}

void Boot::performStandardWait() const {
    _logger("[BOOT] Standard Wait (" + String(TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS) + "ms)");
    delay(TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS);
}

void Boot::performAggressiveWait() const {
    _logger("[BOOT] Aggressive Wait (Heartbeat Active)");
    long startTime = millis();
    long lastHeartbeat = 0;

    while (millis() - startTime < TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS) {
        if (millis() - lastHeartbeat > 2000) {
            _kb->pressKey(KEY_SHIFT);
            delay(50);
            _kb->releaseAll();
            lastHeartbeat = millis();
        }
        delay(100);
    }
}

void Boot::performNavigation(String os) const {
    _logger("[BOOT] Navigating Menu...");

    if (os == OS_NAME_SECONDARY) {
        // Windows
        for (int i = 0; i < GRUB_SECONDARY_OS_POSITION - 1; i++) {
            _kb->pressKey(KEY_DOWN);
            delay(150);
            _kb->releaseAll();
            delay(350);
        }
        delay(500);
        _kb->pressKey(KEY_ENTER);
        delay(150);
        _kb->releaseAll();
    } else {
        // Ubuntu
        _kb->pressKey(KEY_ENTER);
        delay(100);
        _kb->releaseAll();
    }
    _logger("[BOOT] Done");
}

#endif //BOOT_CPP_H
