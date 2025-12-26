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
     * A wake command was received. It's waiting for POST to finish and bootloader to show up.
     */
    BOOTING_UP,
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

    unsigned long _lastSequenceFinishTime = 0;

    void performNavigation();

public:
    Boot(IKeyboard* kb, Logger logger);

    bool isBusy();
    bool isCoolingDown();
    long getCoolDownRemaining();

    // Async contracts
    void startSequence(String os, String strategy);
    void startShutdown();
    void update();
};
#endif
