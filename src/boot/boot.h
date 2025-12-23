#ifndef BOOT_H
#define BOOT_H
#include "IKeyboard.h"
#include <Arduino.h>
#include <functional>
#include "usbkeyboard.h"
#include "../../src/config/config.h"

typedef std::function<void(String)> Logger;

class Boot {
    private:
        IKeyboard* _kb;
        Logger _logger;

    public:
        Boot(IKeyboard* kb, Logger logger) : _kb(kb), _logger(logger) {}

    void selectOS(String os) {
            _logger("[BOOT] Sequence Started for " + os);
            _logger("[BOOT] Total Delay " + String(BOOT_DELAY_IN_MS) + "ms");

            long startTime = millis();
            long lastHeartbeat = 0;

            while ((millis() - startTime) < BOOT_DELAY_IN_MS) {
                if ((millis() - lastHeartbeat) > 2000) {
                    _logger("[BOOT] Heartbeat (Tap Shift) - Keeping USB Alive...");

                    _kb->pressKey(KEY_SHIFT);
                    delay(50);
                    _kb->releaseAll();

                    lastHeartbeat = millis();
                }
                delay(100);
            }

            _logger("[BOOT] Timer done. Starting Navigation.");

            if (os == OS_NAME_SECONDARY) {
                for (int i = 0; i < GRUB_SECONDARY_OS_POSITION - 1; i++) {
                    _logger("[BOOT] KEY_DOWN (" + String(i+1) + ")");
                    _kb->pressKey(KEY_DOWN);
                    delay(150);
                    _kb->releaseAll();
                    delay(350);
                }

                _logger("[BOOT] KEY_ENTER");
                delay(500);
                _kb->pressKey(KEY_ENTER);
                delay(150);
                _kb->releaseAll();

            } else {
                _logger("[BOOT] KEY_ENTER (Default)");
                _kb->pressKey(KEY_ENTER);
                delay(100);
                _kb->releaseAll();
            }

            _logger("[BOOT] Sequence Complete");
        }
};
#endif
