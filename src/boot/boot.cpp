#ifndef BOOT_CPP_H
#define BOOT_CPP_H

#include "boot.h"
#include "../config/config.h"
#include "keyboard/usbkeyboard.h"

Boot::Boot(IKeyboard *kb, Logger logger) : _kb(kb), _logger(std::move(logger)) {
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
    return COOLDOWN_PERIOD_IN_SECONDS - elapsed;
}

void Boot::startSequence(String os, String strategy) {
    if (_state != IDLE && _state != COOLING_DOWN) return;

    _targetOs = os;
    _strategy = strategy;

    _state = BOOTING_UP;
    _stateStartTime = millis();
    _lastHeartbeat = millis();

    _logger("[Boot] Started Async Wait (" + String(TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS) + "ms)");
}

void Boot::startShutdown() {
    _state = SHUTTING_DOWN;
    _stateStartTime = millis();
    _logger("[Shutdown] System Locked for Shutdown...");
}

void Boot::update() {
    unsigned long now = millis();
    unsigned long timeInState = now - _stateStartTime;

    switch (_state) {
        case IDLE:
            break;

        case BOOTING_UP:
            if (timeInState >= TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS) {
                _state = NAVIGATING;
                performNavigation();

                _state = COOLING_DOWN;
                _stateStartTime = millis();
                _logger("[Boot] Sequence Done. Cooling down.");
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
            if (timeInState >= (COOLDOWN_PERIOD_IN_SECONDS * 1000)) {
                _state = IDLE;
                _logger("[Boot] System Ready (Idle)");
            }
            break;

        case SHUTTING_DOWN:
            // Intentional 5 second delay before cool down period starts
            if (timeInState > 5000) {
                _state = COOLING_DOWN;
                _stateStartTime = millis();
                _logger("[Shutdown] Shutdown lock released.");
            }
            break;

            case NAVIGATING:
            break;
    }
}

void Boot::performNavigation() {
    _logger("[Boot] Navigating Menu...");

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
    _logger("[Boot] Success. Selected " + _targetOs);
}

#endif //BOOT_CPP_H
