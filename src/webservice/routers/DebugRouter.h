#ifndef DEBUG_ROUTER_H
#define DEBUG_ROUTER_H

#include <WebServer.h>
#include "../../boot/usbkeyboard.h"

class DebugRouter {
public:
    static void handleType(WebServer &server, USBKeyboard &hwKb) {
        if (!server.hasArg("key")) {
            server.send(400, "text/plain", "Missing 'key'");
            return;
        }

        String keyName = server.arg("key");
        keyName.toLowerCase();
        uint8_t keyCode = 0;

        if (keyName == "shift") keyCode = KEY_SHIFT;
        else if (keyName == "enter") keyCode = KEY_ENTER;
        else if (keyName == "esc") keyCode = KEY_ESCAPE;
        else if (keyName == "win") keyCode = KEY_GUI;
        else if (keyName == "down") keyCode = KEY_DOWN;
        else if (keyName == "up") keyCode = KEY_UP;
        else if (keyName.length() == 1) keyCode = keyName.charAt(0);

        if (keyCode != 0) {
            hwKb.pressKey(keyCode);
            delay(100);
            hwKb.releaseAll();
            server.send(200, "text/plain", "Typed: " + keyName);
        } else {
            server.send(400, "text/plain", "Unknown Key");
        }
    }
};
#endif
