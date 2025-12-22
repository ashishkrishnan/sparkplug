## 📦 Installation & Deployment

Note: A script to flash via CLI is WIP.

### Step 1: Configuration
Edit `src/local.h` to match your network:

```cpp
// Wifi setup
WIFI_SSID     = "YOUR_WIFI_NAME";
WIFI_PASS     = "YOUR_WIFI_PASSWORD";

// Target PC Configuration
LOCAL_OS_NAME_PRIMARY = "ubuntu";
LOCAL_OS_NAME_SECONDARY = "windows";

// order in the grub menu
LOCAL_GRUB_SECONDARY_OS_POSITION = 4 

// LAN IP of your PC
LOCAL_TARGET_PC_IP_ADDRESS  = "192.168.1.100"; 

// Time from Power On -> GRUB Menu
LOCAL_BOOT_DELAY_IN_MS   = 12000;
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
