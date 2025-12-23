<div align="center">

  <img src="sparkplug.png" alt="Sparkplug Logo" width="200" />

  <h1>Sparkplug</h1>

  <p>
    <strong>An internal PC controller built on the ESP32-S3.</strong>
  </p>

  <p>
    <a href="#why">Why?</a> •
    <a href="#capabilities">Capabilities</a> •
    <a href="#hardware">Hardware</a> •
    <a href="#wiring">Wiring</a> •
    <a href="#installation">Installation</a>
  </p>

</div>

<br />

Sparkplug sits inside your computer case, connected to the motherboard's USB and power headers, allowing you to remotely Wake, navigate BIOS/Dual-Boot menu and safely shutdown, all via simple REST APIs

Unlike standard Wake-on-LANs which are unreliable, mostly due to driver-os conflicts and hardware restrictions, the Sparkplug hooks into the motherboard providing **state awareness**, **thermal safety (overheat protection)**, and **hardware-level keyboard emulation** for navigating GRUB/Boot Managers.

Rest APIs:
<br>*Wake up*
<br> `POST/GET http://<sparkplug ip>/wake` (default os)
<br>*<br> if you have a dual boot*
<br> `POST/GET http://<sparkplug ip>/wake?os=windows`
<br> `POST/GET  http://<sparkplug ip>/wake?os=ubuntu`

<br>*Shutdown* : <br>`POST/GET http://<sparkplug ip>/shutdown`

<br>*Health* :<br>`GET http://<sparkplug ip>/health`

<br> Note: API auto-refreshes every 20 seconds configurable or call with query parameter `refresh=50`

```json
{
  "system": {
    "status": "online",
    "uptime_s": 91,
    "uptime_str": "0h 1m 31s",
    "server_time": "22-12-2025 10:20:53 PM"
  },
  "hardware": {
    "chip": "ESP32-S3 Rev 2",
    "free_ram_kb": 223,
    "total_ram_kb": 317,
    "temp_c": 36.8
  },
  "network": {
    "ip": "192.168.0.25",
    "mac": "<mac-address>",
    "signal_dbm": -60
  },
  "logs": [
    "[Syncing with NTP time ] [Sparkplug] Starting system",
    "[22-12-2025 10:19:31 PM] [Sparkplug] WebService Starting",
    "[22-12-2025 10:19:31 PM] [Sparkplug] Service Ready! - All systems go!",
    "[22-12-2025 10:19:31 PM] [Sparkplug] Boot Complete. Ready."
  ]
}
```
---

<a id="capabilities"></a>
## 🚀 Capabilities

* **Reliable Wake up on Lan (WoL):** Remotely power on your PC and automatically type the keystrokes to select your OS (e.g., Windows vs. Ubuntu).
* **Safe Shutdown:** Ensures, triggers are on the power button if the PC is confirmed ON. This prevents accidental power-ups.
* **Thermal Guard:** Monitors internal case temperature. If the ESP32 exceeds **85°C**, it locks out all controls to prevent hardware damage.
* **Realtime Health Monitoring**: Provides observability for hardware state and software sequences.

> [!IMPORTANT] 
> The OS chooser capability requires ESP32-S3 with Native USB OTG" (On-The-Go) support. 
> This makes it an ideal case for pretending it to be an HID in order to choose OS. In later milestone, we will make this modular to flash capabilities based on the board.

---
<a id="hardware"></a>
## 🛠️ Hardware

| Component                 | Specification                                                                                                                                                                                                            | Critical Nuance |
|:--------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------| :--- |
| **Microcontroller**       | ESP32-S3-DevKitC-1-N8R8 <br><br> *Use any ESP32-S3 variant. Remember to update the firmware settings for Flash/RAM sizes (like N16R8). <br><br> **S3 variant is recommended if you want to emulate keyboard behaviour*** | **N8** (8MB Flash) + **R8** (8MB OPI PSRAM). <br> *Note: "OPI PSRAM" setting is mandatory.* |
| **Relay Module**          | 5V Relay (SRD-05VDC-SL-C)                                                                                                                                                                                                | Must support **High-Level Trigger**. <br> *Set the jumper to H.* |
| **Data Cable (optional)** | 9-Pin Header (F) → USB-C/USB-MICRO                                                                                                                                                                                       | Connects to the ESP32's **USB** port (Not UART). |
| **Wiring**                | DuPont Wires                                                                                                                                                                                                             | Female-to-Male (Motherboard) & Female-to-Female (Internal). |

---
<a id="wiring"></a>
## 🔌 Wiring & Assembly

### 1. Relay Configuration
> [!IMPORTANT]
> **CRITICAL:** You must set the jumper on the relay module to **H (High)**.
> Standard "Low" triggers can cause the PC to boot-loop during ESP32 resets. "High" ensures stability.

### 2. Pin Map

| ESP32 Pin | Relay / Motherboard Pin | Description |
| :--- |:------------------------| :--- |
| **5V** | Relay VCC or DC+        | Power for coil. Do not use 3.3V. |
| **GND** | Relay GND or DC-        | Common Ground. |
| **GPIO 4** | Relay IN                | Signal Pin. |
| *(None)* | Relay COM               | Connect to Motherboard **PWR_SW (+)** |
| *(None)* | Relay NO                | Connect to Motherboard **PWR_SW (-)** |

### 3. USB Connection
Connect the 9-pin internal cable to the ESP32 port labeled **USB** (or OTG).
* **Do not** use the UART port, as it cannot emulate a keyboard.

---

## 💻 Software Prerequisites

* Arduino IDE
* **Arduino Libraries:**
  * `ESPPing` (Install via Arduino Library Manager)
  * *Note: ESP32, WiFi, and ESPmDNS are built-in.*

---

<a id="installation"></a>
## 📦 Installation & Deployment

Note: A script to flash via CLI is WIP.

### Step 1: Configuration
Edit `src/local.h` to match your network:

```cpp
// Wifi setup
WIFI_SSID     = "YOUR_WIFI_NAME";
WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Target PC Configuration
LOCAL_OS_NAME_PRIMARY = "ubuntu";
LOCAL_OS_NAME_SECONDARY = "windows";

// order in the grub menu
LOCAL_GRUB_SECONDARY_OS_POSITION = 4 

// LAN IP of your PC
LOCAL_TARGET_PC_IP_ADDRESS  = "192.168.1.100"; 

// Time from Power On -> GRUB Menu
LOCAL_BOOT_DELAY_IN_MS   = 12000;

LOCAL_TIME_ZONE = "IST-5:30";
```

### Step 2: Manual Flashing via Arduino IDE
Download Arduino IDE and follow [Instructions](/docs/flashing-guidelines.md) here.

<a id="why"></a>
## Why? ##
-----------------------

Based on my personal "exhausting" troubleshooting experience with the my ASUS AM5 board and Linux, here is why standard Wake-on-LAN (WoL) is fundamentally unreliable and pushed me to towards a hardware solution:

#### My Experience:

*   **OS Dependency**: WoL failed because the Linux driver and ASUS BIOS could not agree on power states during shutdown. It consistently went to D3Cold state, even when some of the scripts asked not to. If the OS does not perfectly arm the hardware, the feature fails completely. To my surprise, Windows drivers worked well.

*   **Blind Operation**: When WoL failed, there was zero feedback or error messaging. The Magic Packet is a strictly one-way signal, making remote diagnosis impossible.


#### General Limitations

*   **No Crash Recovery**: WoL can only wake a system from a clean shutdown. It is useless if the operating system is frozen or hung.

*   **Fragile Prerequisites**: It requires a specific chain of BIOS settings (ErP, Native Power Management) and driver configurations. These are easily reset or broken by firmware updates and dual-boot conflicts.


#### Hence, Hardware Relay

*   **OS Independence**: It physically triggers the motherboard power headers, bypassing drivers, BIOS tables, and kernel versions entirely.

*   **State Verification**: Unlike WoL, it can actively verify if the machine is actually on or off (via ping) before attempting an action.

*   **Force Reset**: It can simulate a long-press on the power button to hard-reset a frozen machine—something software-based methods simply cannot do.

