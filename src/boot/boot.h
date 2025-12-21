#ifndef BOOT_H
#define BOOT_H
#include "IKeyboard.h"
#include <Arduino.h>

#include "../../src/config/config.h"

constexpr uint8_t KEY_ENTER = 0xB0;
constexpr uint8_t KEY_DOWN = 0xD9;

class Boot {
    private:
        IKeyboard* _kb;
    public:
        Boot(IKeyboard* kb) : _kb(kb) {}

        void selectOS(String os) {
            delay(BOOT_DELAY_IN_MS);
            _kb->begin();

            delay(1000);

            if (os == OS_NAME_SECONDARY) {
                for (int i = 0; i < GRUB_SECONDARY_OS_POSITION - 1; i++) {
                    _kb->pressKey(KEY_DOWN);
                    delay(100);
                    _kb->releaseAll();
                    delay(100);
                }
                _kb->pressKey(KEY_ENTER);
            } else {
                _kb->pressKey(0xD9);
            }
            delay(100);
            _kb->releaseAll();
            _kb->end();
        }
};
#endif //BOOT_H
