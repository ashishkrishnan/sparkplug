**Sparkplug**
=======================

**Sparkplug** is proposed to be a internal PC controller built on the **ESP32-S3**. It sits inside the computer case, connected to the motherboard's USB and power headers, allowing you to remotely wake, safe-shutdown, and navigate BIOS/Dual-Boot menus via a simple REST API.

Unlike standard Wake-on-LAN which is unreliable, Sparkplug provides **state awareness** (Ping-Gating), **thermal safety** (overheat protection), and **hardware-level keyboard emulation** for navigating GRUB/Boot Managers.

**Proposed Capabilities**
-----------------------

*   **Wake:** Remotely power on your PC and automatically type the keystrokes to select your OS via an API (/wake?os=ubuntu) (e.g., Windows vs. Ubuntu).
    
*   **Safe Shutdown:** Use a "Ping Gate" to ensure it only triggers the power button if the PC is confirmed **ON** to prevents accidental power-ups.
    
*   (experimental) **Thermal Guard:** Proposed monitoring of internal case temperature. If the ESP32 exceeds 85°C, it locks out all controls to prevent hardware damage.
    
*   **No visbile cables:** Connects via internal USB header on the motherboard. No external cables or dongles.

*   **Emergency Mode:** Force reset in situations when you appear to have lost control due to lag or screen freeze.


**🛑 Why?**
-----------------------

Based on my personal "exhausting" troubleshooting experience with the my ASUS AM5 board and Linux, here is why standard Wake-on-LAN (WoL) is fundamentally unreliable and pushed me to towards a hardware solution:

#### My Experience:

*   **OS Dependency**WoL failed because the Linux driver and ASUS BIOS could not agree on power states during shutdown. It consistently went to D3Cold state, even when some of the scripts asked not to. If the OS does not perfectly arm the hardware, the feature fails completely. To my surprise, Windows drivers worked well.
    
*   **Blind Operation**When WoL failed, there was zero feedback or error messaging. The Magic Packet is a strictly one-way signal, making remote diagnosis impossible.
    

#### General Limitations

*   **No Crash Recovery**WoL can only wake a system from a clean shutdown. It is useless if the operating system is frozen or hung.
    
*   **Fragile Prerequisites**It requires a specific chain of BIOS settings (ErP, Native Power Management) and driver configurations. These are easily reset or broken by firmware updates and dual-boot conflicts.
    

#### Hence, Hardware Relay

*   **OS Independence**It physically triggers the motherboard power headers, bypassing drivers, BIOS tables, and kernel versions entirely.
    
*   **State Verification**Unlike WoL, it can actively verify if the machine is actually on or off (via ping) before attempting an action.
    
*   **Force Reset**It can simulate a long-press on the power button to hard-reset a frozen machine—something software-based methods simply cannot do.
