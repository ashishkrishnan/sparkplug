#ifndef SAFETY_H
#define SAFETY_H
#include "ISystemHealth.h"
#include "config/config.h"

class Safety {
    private:
        ISystemHealth* _health;
    public:
        Safety(ISystemHealth* h) : _health(h) {}

        bool isSafeToOperate() {
            return _health->getInternalTemp() < MAX_TEMP_C;
        }

        bool isSafeShutdownAllowed() {
            if (!isSafeToOperate()) return false;

            return _health->isTargetPcAlive(TARGET_PC_IP_ADDRESS);
        }
};
#endif //SAFETY_H
