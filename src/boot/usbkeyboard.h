#ifndef USBKEYBOARD_H
#define USBKEYBOARD_H
#include "IKeyboard.h"
#include "USB.h"
#include "USBHIDKeyboard.h"

class USBKeyboard : public IKeyboard {
    private:
        USBHIDKeyboard _hid;
    public:
        void begin() override {
            USB.begin();
            _hid.begin();
        }

        void end() override {
            _hid.end();
        }

        void pressKey(uint8_t key) override {
            _hid.press(key);
        }

        void releaseAll() override {
            _hid.releaseAll();
        }
};
#endif //USBKEYBOARD_H
