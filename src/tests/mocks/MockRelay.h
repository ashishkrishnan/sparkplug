#ifndef MOCKRELAY_H
#define MOCKRELAY_H
#include <Arduino.h>
#include "../src/power/IRelay.h"

class MockRelay : public IRelay {
    public:
        String log = "";

        void configure() override { log += "[CONF]"; }
        void setHigh() override { log += "[HIGH]"; }
        void setLow() override { log += "[LOW]"; }
};

#endif //MOCKRELAY_H
