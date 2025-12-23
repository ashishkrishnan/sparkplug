## 📦 Installation & Deployment

Note: A script to flash via CLI is WIP.

### Step 1: Configuration
Edit `src/local.h` to match your network:

```cpp
static const char* LOCAL_WIFI_SSID = "SSID";
static const char* LOCAL_WIFI_PASSWORD = "foo-bar-password";

static constexpr int LOCAL_TIME_TAKEN_TO_REACH_BOOT_MENU = 9000;

static const char* LOCAL_OS_NAME_PRIMARY = "ubuntu";
static const char* LOCAL_OS_NAME_SECONDARY = "windows";

static constexpr int LOCAL_GRUB_SECONDARY_OS_POSITION = 4;

static const char* LOCAL_TARGET_PC_IP_ADDRESS = "192.168.1.7";

const char* LOCAL_TIME_ZONE = "IST-5:30"; // e.g "IST-5:30" which is Asia/Kolkata
```
> [!IMPORTANT]
> Routers have an option to permanent reserve DHCP local IP to your devices. Use to ensure that the target PC does have a new ip.

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

##### ESP32-S3 Arduino IDE Configuration
> [!IMPORTANT]
>These settings are specifically tuned for an **ESP32-S3 N8R8** (8MB Flash, 8MB PSRAM) using the native USB port for Serial and Keyboard features. Please change these settings based on your board.

| Setting Menu | Select This Option | Reason / Notes |
| --- | --- | --- |
| **Board** | ESP32S3 Dev Module | The base definition for S3. |
| **USB CDC On Boot** | **Enabled** | **Crucial.** Allows Serial Monitor to work over the native USB port. |
| **CPU Frequency** | 240MHz (WiFi) | Max speed. |
| **Core Debug Level** | None | Keeps logs clean. |
| **USB DFU On Boot** | Disabled | Standard setting. |
| **Erase All Flash** | Disabled | Keep enabled only if you want to wipe Wi-Fi credentials. |
| **Events Run On** | Core 1 | Standard. |
| **Flash Mode** | QIO 80MHz | Fast flash speed. |
| **Flash Size** | 8MB (64Mb) | Matches your **N8** spec. |
| **JTAG Adapter** | Disabled | Unless you are using a debugger probe. |
| **Arduino Runs On** | Core 1 | Standard. |
| **USB Firmware MSC** | Disabled | Standard. |
| **Partition Scheme** | 8M with SPIFFS | Uses your full 8MB of storage. |
| **PSRAM** | **OPI PSRAM** | **CRITICAL.** Matches your **R8** spec. |
| **Upload Mode** | UART0 / Hardware CDC | Ensures upload works over the cable. |
| **USB Mode** | **Hardware CDC and JTAG** | **CRITICAL.** Enables the Keyboard/HID features. |

---

#### 3\. Edit Configuration

1.  Open the project file (e.g., Sparkplug.ino) in Arduino IDE.

2.  Navigate to the local.h tab (or file).

3.  Make sure you have the configs updated as per details above.


#### 4\. Upload

1. Connect your Sparkplug module to your PC using the port labeled **UART**.

2. Hold the **BOOT** button on the ESP32-S3.

3. Press and release the **RESET** button (while still holding BOOT).

4. Release the **BOOT** button. _This forces the board into "Download Mode"._

5. Click on Compile

6. start uploading

