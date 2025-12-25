#ifndef BOOT_H
#define BOOT_H

#include "IKeyboard.h"
#include <functional>

typedef std::function<void(String)> Logger;

class Boot {
private:
    IKeyboard* _kb;
    Logger _logger;

    bool _isBusy = false;
    unsigned long _lastSequenceFinishTime = 0;

    void performStandardWait() const;
    void performAggressiveWait() const;
    void performNavigation(String os) const;

public:
    Boot(IKeyboard* kb, Logger logger);

    bool isBusy();
    bool isCoolingDown();
    long getCoolDownRemaining();

    // TODO(ak) make this non-blocking
    void selectOS(const String &os, const String &strategy);
};

#endif
