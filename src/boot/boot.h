#ifndef BOOT_H
#define BOOT_H
#include "IKeyboard.h"
#include <Arduino.h>
#include <functional>
#include <utility>
#include "usbkeyboard.h"
#include "../../src/config/config.h"

typedef std::function<void(String)> Logger;

class Boot {
    private:
        IKeyboard* _kb;
        Logger _logger;
        bool _isBusy;
        unsigned long _lastSequenceFinishTime = 0;


    public:
        Boot(IKeyboard* kb, Logger logger) : _kb(kb), _logger(std::move(logger)) {}

        bool isBusy() { return _isBusy; }

        bool isCoolingDown() {
            if (_isBusy) return false;

            unsigned long elapsed = (millis() - _lastSequenceFinishTime) / 1000;
            return (elapsed < POST_BOOT_COOL_DOWN_IN_SECONDS);
        }

        long getCoolDownRemaining() {
            if (!isCoolingDown()) return 0;
            unsigned long elapsed = (millis() - _lastSequenceFinishTime) / 1000;
            return POST_BOOT_COOL_DOWN_IN_SECONDS - elapsed;
        }

        void selectOS(const String &os, const String &strategy) {
            if (_isBusy) return; // internal double-check
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

    private:
        void performStandardWait() const {
            _logger("[BOOT] Standard Wait (" + String(TIME_TAKEN_TO_REACH_BOOT_MENU) + "ms)");
            delay(TIME_TAKEN_TO_REACH_BOOT_MENU);
        }

        void performAggressiveWait() const {
            _logger("[BOOT] Aggressive Wait (Heartbeat Active)");
            long startTime = millis();
            long lastHeartbeat = 0;

            while (millis() - startTime < TIME_TAKEN_TO_REACH_BOOT_MENU) {
                if (millis() - lastHeartbeat > 2000) {
                    _kb->pressKey(KEY_SHIFT);
                    delay(50);
                    _kb->releaseAll();
                    lastHeartbeat = millis();
                }
                delay(100);
            }
        }

        void performNavigation(String os) const {
            _logger("[BOOT] Navigating Menu...");

            if (os == OS_NAME_SECONDARY) {
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
            _logger("[BOOT] Done");
        }
};
#endif
