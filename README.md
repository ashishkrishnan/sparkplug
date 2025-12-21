<div align="center">

  <img src="sparkplug.png" alt="Sparkplug Logo" width="200" />

  <h1>Project Sparkplug</h1>

  <p>
    <strong>An internal PC controller built on the ESP32-S3.</strong>
  </p>

  <p>
    <a href="#capabilities">Capabilities</a> •
    <a href="#hardware">Hardware</a> •
    <a href="#wiring">Wiring</a> •
    <a href="#installation">Installation</a> •
    <a href="#why">Why?</a>
  </p>

</div>

<br />

Sparkplug sits inside your computer case, connected to the motherboard's USB and Power headers, allowing you to remotely Wake, Safe-Shutdown, and navigate BIOS/Dual-Boot menus via a simple REST API.

Unlike standard Wake-on-LAN, Sparkplug provides **state awareness (Ping-Gating)**, **thermal safety (overheat protection)**, and **hardware-level keyboard emulation** for navigating GRUB/Boot Managers.

Rest APIs:

<br>*Wake up*
<br> `POST http://<sparkplug ip>/wake`
<br> `POST http://<sparkplug ip>/wake?os=windows`
<br> `POST http://<sparkplug ip>/wake?os=ubuntu`

<br>*Shutdown* : <br>`POST http://<sparkplug ip>/shutdown`

<br>*Health*  : <br>`GET http://<sparkplug ip>/health`

---

<a id="capabilities"></a>
## 🚀 Capabilities

* **Reliable Wake up on Lan:** Remotely power on your PC and automatically type the keystrokes to select your OS (e.g., Windows vs. Ubuntu).
* **Safe Shutdown:** Uses a "Ping Gate" to ensure it only triggers the power button if the PC is confirmed ON. This prevents accidental power-ups.
* **Thermal Guard:** Monitors internal case temperature. If the ESP32 exceeds **85°C**, it locks out all controls to prevent hardware damage.
* **(optional) No cables setup:** Connects via internal USB header on the motherboard. No external cables or dongles.

---
<a id="hardware"></a>
## 🛠️ Hardware

| Component                 | Specification                                                                                                                                                                                                            | Critical Nuance |
|:--------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------| :--- |
| **Microcontroller**       | ESP32-S3-DevKitC-1-N8R8 <br><br> *Use any ESP32-S3 variant. Remember to update the firmware settings for Flash/RAM sizes (like N16R8). <br><br> **S3 variant is recommended if you want to emulate keyboard behaviour*** | **N8** (8MB Flash) + **R8** (8MB OPI PSRAM). <br> *Note: "OPI PSRAM" setting is mandatory.* |
| **Relay Module**          | 5V Relay (SRD-05VDC-SL-C)                                                                                                                                                                                                | Must support **High-Level Trigger**. <br> *Set the jumper to H.* |
| **Data Cable (optional)** | 9-Pin Header (F) → USB-C                                                                                                                                                                                                 | Connects to the ESP32's **USB** port (Not UART). |
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

* **Python 3.x:** (with pip installed)
* **Arduino CLI:** [Installation Guide](https://arduino.github.io/arduino-cli/latest/installation/)
* **Python Dependencies:**
    ```bash
    pip install pyserial esptool
    ```
* **Arduino Libraries:**
  * `ESPPing` (Install via Arduino Library Manager)
  * *Note: ESP32, WiFi, and ESPmDNS are built-in.*

---

<a id="installation"></a>
## 📦 Installation & Deployment


### Step 1: Configuration
Edit `src/Config.h` to match your network:

```cpp
const char* WIFI_SSID     = "YOUR_WIFI_NAME";
const char* WIFI_PASS     = "YOUR_WIFI_PASSWORD";
const char* PC_STATIC_IP  = "192.168.1.100"; // LAN IP of your PC
const int BOOT_DELAY_MS   = 12000;           // Time from Power On -> GRUB Menu
```

### Manual Flashing via Arduino IDE

#### 1\. Setup Arduino IDE

1.  **Install ESP32 Board Support:**

*   Go to **File** -> **Preferences**.

*   In "Additional Board Manager URLs", add:https://espressif.github.io/arduino-esp32/package\_esp32\_index.json

*   Go to **Tools** -> **Board** -> **Boards Manager**.

*   Search for **"esp32"** (by Espressif Systems) and install the latest version (3.0.x or higher recommended).

2.  **Install Dependencies:**

*   Go to **Tools** -> **Manage Libraries**.

*   Search for and install: **ESPPing** (by Dvvrall).


#### 2\. Board Configuration (Critical)

Because you are using the **N8R8** (8MB Flash / 8MB OPI PSRAM) version of the S3, these settings are mandatory to prevent boot loops.

Go to the **Tools** menu and match these settings:

*   **Board:** ESP32S3 Dev Module

*   **USB CDC On Boot:** Enabled (Required to see logs via the native USB port)

*   **CPU Frequency:** 240MHz (WiFi)

*   **Flash Mode:** QIO 80MHz

*   **Flash Size:** 8MB (64Mb)

*   **Partition Scheme:** 8M Flash (3MB APP / 1.5MB SPIFFS)

*   **PSRAM:** OPI PSRAM **(Critical!)**

*   **Upload Mode:** USB-OTG CDC (JTAG)


#### 3\. Edit Configuration

1.  Open the project file (e.g., Sparkplug.ino) in Arduino IDE.

2.  Navigate to the Config.h tab (or file).

3.  C++const char\* WIFI\_SSID = "MyHomeNetwork";const char\* WIFI\_PASS = "SuperSecretPassword";


#### 4\. Upload

1.  Connect your Sparkplug module to your PC using the port labeled **USB** (not UART).

2.  Select the correct COM port under **Tools** -> **Port**.

*   _Note: It might appear as "ESP32S3 Native USB"._

1. Click the **Upload** (Right Arrow) button.


### Troubleshooting the Upload

If the upload fails or the board disconnects:

1.  Hold the **BOOT** button on the ESP32-S3.

2.  Press and release the **RESET** button (while still holding BOOT).

3.  Release the **BOOT** button. _This forces the board into "Download Mode"._

4. Try clicking **Upload** again.

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

