#ifndef BOOT_CPP_H
#define BOOT_CPP_H

#include "boot.h"
#include "../config/config.h"
#include "usbkeyboard.h"

Boot::Boot(IKeyboard *kb, Logger logger) : _kb(kb), _logger(std::move(logger)) {
    _isBusy = false;
    _lastSequenceFinishTime = 0;
    _state = IDLE;
}

bool Boot::isBusy() {
    return (_state != IDLE && _state != COOLING_DOWN);
}

bool Boot::isCoolingDown() {
    return (_state == COOLING_DOWN);
}

long Boot::getCoolDownRemaining() {
    if (!isCoolingDown()) return 0;
    unsigned long elapsed = (millis() - _lastSequenceFinishTime) / 1000;
    return POST_BOOT_COOL_DOWN_IN_SECONDS - elapsed;
}

String Boot::getStateName() {
    switch(_state) {
        case IDLE: return "IDLE";
        case WAITING_FOR_BOOT: return "WAITING_BIOS";
        case NAVIGATING: return "NAVIGATING";
        case SHUTTING_DOWN: return "SHUTTING_DOWN";
        case COOLING_DOWN: return "COOLING_DOWN";
        default: return "UNKNOWN";
    }
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

void Boot::startSequence(String os, String strategy) {
    if (_state != IDLE && _state != COOLING_DOWN) return;

    _targetOs = os;
    _strategy = strategy;

    _state = WAITING_FOR_BOOT;
    _stateStartTime = millis();
    _lastHeartbeat = millis();

    _logger("[BOOT] Started Async Wait (" + String(TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS) + "ms)");
}

void Boot::startShutdown() {
    _state = SHUTTING_DOWN;
    _stateStartTime = millis();
    _logger("[SEQ] System Locked for Shutdown...");
}

void Boot::update() {
    unsigned long now = millis();
    unsigned long timeInState = now - _stateStartTime;

    switch (_state) {
        case IDLE:
            break;

        case WAITING_FOR_BOOT:
            if (timeInState >= TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS) {
                _state = NAVIGATING;
                performNavigation();

                _state = COOLING_DOWN;
                _stateStartTime = millis();
                _logger("[BOOT] Sequence Done. Cooling down.");
                return;
            }

            if (_strategy == "aggressive") {
                if (now - _lastHeartbeat > 2000) {
                    _kb->pressKey(KEY_SHIFT);
                    delay(50); // Keep a delay between key presses.
                    _kb->releaseAll();
                    _lastHeartbeat = now;
                }
            }
            break;

        case COOLING_DOWN:
            if (timeInState >= (POST_BOOT_COOL_DOWN_IN_SECONDS * 1000)) {
                _state = IDLE;
                _logger("[BOOT] System Ready (Idle)");
            }
            break;

        case SHUTTING_DOWN:
            // Intentional 5 second delay before cool down period starts
            if (timeInState > 5000) {
                _state = COOLING_DOWN;
                _stateStartTime = millis();
                _logger("[SEQ] Shutdown lock released.");
            }
            break;

            case NAVIGATING:
            break;
    }
}

void Boot::performNavigation() {
    _logger("[BOOT] Navigating Menu...");

    if (_targetOs == OS_NAME_SECONDARY) {
        for (int i = 0; i < GRUB_SECONDARY_OS_POSITION - 1; i++) {
            _kb->pressKey(KEY_DOWN);
            delay(150); _kb->releaseAll(); delay(350);
        }
        delay(500);
        _kb->pressKey(KEY_ENTER);
        delay(150); _kb->releaseAll();
    } else {
        _kb->pressKey(KEY_ENTER);
        delay(100); _kb->releaseAll();
    }
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
