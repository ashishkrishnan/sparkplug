#ifndef USBKEYBOARD_H
#define USBKEYBOARD_H
#include "IKeyboard.h"
#include "USB.h"
#include "USBHIDKeyboard.h"

constexpr uint8_t KEY_ENTER = 0xB0;
constexpr uint8_t KEY_ESCAPE = 0xB1;
constexpr uint8_t KEY_BACK = 0xB2;
constexpr uint8_t KEY_RIGHT = 0xD7;
constexpr uint8_t KEY_LEFT  = 0xD8;
constexpr uint8_t KEY_DOWN  = 0xD9;
constexpr uint8_t KEY_UP    = 0xDA;
constexpr uint8_t KEY_SHIFT = 0x81;
constexpr uint8_t KEY_GUI   = 0x83; // Windows Key

class USBKeyboard : public IKeyboard {
private:
    USBHIDKeyboard _hid;
public:
    void init() {
        USB.begin();
        _hid.begin();
    }

    void pressKey(uint8_t key) override {
        _hid.press(key);
    }

    void releaseAll() override {
        _hid.releaseAll();
    }
};
#endif
