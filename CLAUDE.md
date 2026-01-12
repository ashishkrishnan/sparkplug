# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Sparkplug is an ESP32-S3 based internal PC controller for homelabs. It provides reliable Wake-on-LAN, OS selection via HID keyboard emulation, and safe shutdown capabilities through REST APIs. The device connects to motherboard USB and power headers, using a relay for power control and USB OTG for keyboard emulation.

## Build & Flash Commands

This is an Arduino IDE project. There is no CLI build system yet.

**Dependencies:**
- Arduino IDE with ESP32 board support (Espressif package 3.0.x+)
- ESPPing library (install via Arduino Library Manager)

**Flash procedure:**
1. Connect ESP32-S3 via UART port
2. Hold BOOT button, press RESET, release BOOT (enters Download Mode)
3. Compile and upload via Arduino IDE

**Run tests:**
Define `RUN_TESTS_ON_BOOT` preprocessor macro before compiling to run the test suite on device boot.

## Architecture

```
sparkplug.ino          # Entry point: setup() and loop()
src/
  boot/                # Boot sequence and keyboard navigation
    Boot               # State machine (IDLE→BOOTING_UP→NAVIGATING→COOLING_DOWN)
    keyboard/          # IKeyboard interface + USBKeyboard HID implementation
  power/               # Relay control
    Power              # Pulse trigger for power button simulation
    IRelay/Relay       # Hardware abstraction
  safety/              # Thermal and connectivity guards
    Safety             # Checks temp < 85°C and target PC ping status
    Health             # ISystemHealth implementation
  connectivity/        # WiFi + mDNS setup
  webservice/          # HTTP API server
    routers/           # Wake, Shutdown, Health, Debug endpoints
  config/              # Configuration files (wifi.h, config.h)
  tests/               # Test runner and mocks
    mocks/             # MockKeyboard, MockHealth, MockRelay
```

**Key patterns:**
- Dependency injection via interfaces (IKeyboard, IRelay, ISystemHealth) enables testing with mocks
- Async boot sequence uses state machine with `update()` polling in main loop
- Callbacks connect webservice routes to main orchestration functions

## Configuration

Edit before flashing:

- `src/config/wifi.h` - WiFi credentials
- `src/config/config.h` - Hostname, timezone, target PC IP, boot timing, GPIO pins

## REST API

- `GET /wake` - Power on (optional: `?os=windows|ubuntu`, `?strategy=aggressive`, `?force=true`)
- `GET /shutdown` - Safe shutdown (only if target PC responds to ping)
- `GET /health` - System status, temperature, logs (optional: `?refresh=N`)
- `GET /debug/type?key=X` - Test HID keyboard emulation

## Hardware Notes

- ESP32-S3 with USB OTG required for keyboard emulation
- GPIO 4 controls the relay (High-level trigger, set jumper to H)
- Thermal lockout at 85°C
- Cooldown period prevents rapid wake/shutdown cycles
