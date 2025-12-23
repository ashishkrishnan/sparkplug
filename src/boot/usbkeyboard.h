#ifndef USBKEYBOARD_H
#define USBKEYBOARD_H
#include "IKeyboard.h"
#include "USB.h"
#include "USBHIDKeyboard.h"

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
