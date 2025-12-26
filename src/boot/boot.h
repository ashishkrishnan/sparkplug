#ifndef BOOT_H
#define BOOT_H

#include "IKeyboard.h"
#include <functional>

typedef std::function<void(String)> Logger;

enum BootState {
    /**
     * Waiting for commands.
     */
    IDLE,
    /**
     * A wake command was received. It's waiting for POST and bootloader to show up.
     */
    WAITING_FOR_BOOT,
    /**
     * The OS is being selected (primary or secondary)
     */
    NAVIGATING,
    /**
     * A shutdown command was received.
     */
    SHUTTING_DOWN,
    /**
     * A cool-down period after a successful wake or shutdown.
     */
    COOLING_DOWN
};

class Boot {
private:
    IKeyboard* _kb;
    Logger _logger;
    BootState _state = IDLE;
    unsigned long _stateStartTime = 0;
    unsigned long _lastHeartbeat = 0;
    String _targetOs;
    String _strategy;


    // TODO(ak) - to be deprecated
    bool _isBusy = false;
    unsigned long _lastSequenceFinishTime = 0;

    // TODO(ak) - to be deprecated
    void performStandardWait() const;
    // TODO(ak) - to be deprecated
    void performAggressiveWait() const;
    // TODO(ak) - to be deprecated
    void performNavigation(String os) const;
    void performNavigation();

public:
    Boot(IKeyboard* kb, Logger logger);

    bool isBusy();
    bool isCoolingDown();
    long getCoolDownRemaining();
    String getStateName();

    // TODO(ak) to be deprecated
    void selectOS(const String &os, const String &strategy);

    // Async contracts
    void startSequence(String os, String strategy);
    void startShutdown();
    void update();
};
#endif
