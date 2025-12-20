#ifndef IKEYBOARD_H
#define IKEYBOARD_H
#include <Arduino.h>

class IKeyboard {
    public:
        virtual void begin() = 0;

        virtual void end() = 0;

        virtual void pressKey(uint8_t key) = 0;

        virtual void releaseAll() = 0;
};

#endif //IKEYBOARD_H
