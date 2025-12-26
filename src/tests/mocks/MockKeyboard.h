#ifndef MOCKKEYBOARD_H
#define MOCKKEYBOARD_H
#include <Arduino.h>
#include "../../boot/keyboard/IKeyboard.h"

class MockKeyboard : public IKeyboard {
    public:
        String log = "";

        void pressKey(uint8_t key) override {
            log += "[KEY:" + String(key, HEX) + "]";
        }

        void releaseAll() override {
            log += "[REL]";
        }
};
#endif //MOCKKEYBOARD_H
