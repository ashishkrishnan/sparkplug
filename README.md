<div align="center">

  <img src="sparkplug.png" alt="Sparkplug Logo" width="200" />

  <h1>Sparkplug</h1>

  <p>
    <strong>An internal PC controller built on the ESP32-S3 for your homelab.</strong>
  </p>

  <p>
    <a href="#why">Why?</a> •
    <a href="#capabilities">Capabilities</a> •
    <a href="#api">API</a> •
    <a href="#hardware">Hardware</a> •
    <a href="#wiring">Wiring</a> •
    <a href="#installation">Installation</a>
  </p>

</div>

<br />

Sparkplug sits inside your computer case, connected to the motherboard's USB and power headers, allowing you to remotely Wake, navigate BIOS/Dual-Boot menu and safely shutdown, all via simple REST APIs

Unlike standard Wake-on-LANs which are unreliable, mostly due to driver-os conflicts and hardware restrictions, the Sparkplug hooks into the motherboard providing **state awareness**, **thermal safety (overheat protection)**, and **hardware-level keyboard emulation** for navigating GRUB/Boot Managers.

---

<a id="capabilities"></a>
## 🚀 Capabilities

* **Reliable Wake-on-LAN (WoL):** Remotely power on your PC over Wifi (WLAN), which triggers a relay to power on via the motherboard headers.
* **Reliable OS selector:** Use predefined keyboard input to navigate your primary or secondary os (e.g ubuntu or windows)
* **Shutdown:** Safely shuts down your PC.
* **Thermal Guard:** Monitors internal temperature. If the Sparkplug exceeds **85°C**, it locks out all controls to prevent hardware damage.
* **Safety protocols:** Prevents any accidental shutdown or wake-ups if Sparkplug detects an ongoing sequence or is in cooldown period.
* **Realtime Health Monitoring**: Provides observability for hardware state and software sequences.
* **Debug Mode**: Has a debug mode for testing if your motherboard or OS settings are HID compliant.

> [!IMPORTANT] 
> The OS selector capability requires ESP32-S3 with Native USB OTG (On-The-Go) support. 
> This makes it an ideal case for pretending it to be an HID in order to choose OS. In a later milestone, we will make this modular to flash capabilities based on the board.

---
<a id="api"></a>
## APIs:
#### Wake Up
`http://<sparkplug>/wake` (default os)
<br><br> if you have a dual boot
<br> `http://<sparkplug>/wake?os=windows`
<br> `http://<sparkplug>/wake?os=ubuntu`

- Use `strategy=aggressive` if HID is disabled during POST (power on self test) for some motherboards.
- Use `force=true` to wake up during the cooldown period (configurable time period after a recent wake/shutdown)

#### Shutdown
`http://<sparkplug>/shutdown`

#### Health:
`http://<sparkplug>/health`

Note: API auto-refreshes every 30 seconds configurable or call with query parameter `refresh=50`

<br>*Debug*: <br>`http://<sparkplug>/debug/type?key=a`

Note: Use this if you want to test if the Sparkplug is getting detected as a keyboard.

> [!IMPORTANT]
> Check [API Documentation](/docs/api-docs.md) for more information
****


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

### Step 1: Your Wifi Name & Password
Edit `src/config/wifi.h` to match your network:

```cpp
static const char* LOCAL_WIFI_SSID = "<Your Wifi SSID>";
static const char* SECRET_WIFI_PASSWORD = "<Your Wifi Password>";
```

### Step 2: Configure & review your User & Advanced configurations
Edit `src/config/config.h` to match your network:

```cpp
// Configure your host name
static const char* HOSTNAME = "sparkplug";

// Set your locale
static const char* TIME_ZONE     = "IST-5:30"; // POSIX timezone for Asia/Kolkata

// Your target PC Config
static const char* OS_NAME_PRIMARY = "ubuntu";
static const char* OS_NAME_SECONDARY = "windows";
static constexpr int TIME_TAKEN_TO_REACH_BOOT_MENU_IN_MILLIS = 19000;
static constexpr int GRUB_SECONDARY_OS_POSITION = 5;
static const char* TARGET_PC_IP_ADDRESS = "192.168.0.10";
```

### Step 3: Manual Flashing via Arduino IDE
Download Arduino IDE and follow [Instructions](/docs/flashing-guidelines.md) here.

<a id="why"></a>
## Why? ##
-----------------------

For my HomeLabs, I was exploring Wake-on-LAN capability to wake up a Custom built PC (running ubuntu/windows). Idea was to wake it up, do some heavy computation (local models or image processing) and shutdown safely. It is a critical step on my homelab setup to conserve energy for all my future goals.
 
Even for KVM over IP use-cases, a reliable WOL is a necessity.

When I started diving in, it turned out to be a black hole of endless issues.

Based on my personal "exhausting" troubleshooting experience with the ASUS AM5 board (custom configuration) and Linux, here is why standard Wake-on-LAN (WoL) is fundamentally unreliable and pushed me to towards a hardware solution:

#### My Experience:

*   **OS Dependency**: WoL would fail because the Linux driver and ASUS BIOS could not agree on power states during shutdown. It consistently went to D3Cold state, even when some of the scripts asked not to. If the OS does not perfectly arm the hardware, the feature fails completely. To my surprise, Windows drivers worked well.

*   **Blind Operation**: When WoL failed, there was zero feedback or error messaging. The Magic Packet is a strictly one-way signal, making remote diagnosis impossible.


#### General Limitations

*   **No Crash Recovery**: WoL can only wake a system from a clean shutdown. It is useless if the operating system is frozen or hung.

*   **Fragile Prerequisites**: It requires a specific chain of BIOS settings (ErP, Native Power Management) and driver configurations. These are easily reset or broken by firmware updates and dual-boot conflicts.


#### Hence, Hardware Relay

*   **OS Independence**: It physically triggers the motherboard power headers, bypassing drivers, BIOS tables, and kernel versions entirely.

*   **State Verification**: Unlike WoL, it can actively verify if the machine is actually on or off (via ping) before attempting an action.

*   **Force Reset**: It can simulate a long-press on the power button to hard-reset a frozen machine—something software-based methods simply cannot do.
