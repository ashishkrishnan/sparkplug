#ifndef ISYSTEMHEALTH_H
#define ISYSTEMHEALTH_H

class ISystemHealth {
    public:
        virtual float getInternalTemp() = 0;

        virtual bool isTargetPcAlive(const char* ip) = 0;
};

#endif //ISYSTEMHEALTH_H
