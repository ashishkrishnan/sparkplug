## APIs docs:

<br>*Wake up*
<br> `http://<sparkplug ip>/wake` (default os)

<br>*<br> if you have a dual boot*
<br> `http://<sparkplug>/wake?os=windows`
<br> `http://<sparkplug>/wake?os=ubuntu`
<br> `http://<sparkplug>/wake?os=windows&strategy=standard`
<br> `http://<sparkplug>/wake?os=windows&strategy=standard&force=true`

- Use `strategy=aggressive` if HID is disabled or ignoring key stokes during POST (power on self test) for some motherboards.
- Use `force=true` to wake up during the cool down period (configurable time period after a recent wake/shutdown)

<br>*Shutdown* : <br>`http://<sparkplug>/shutdown`

<br>*Health* :<br>`http://<sparkplug>/health`

Note: API auto-refreshes every 20 seconds configurable or call with query parameter `refresh=50`

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
<br>*Debug*: <br>`http://<sparkplug>/debug/type?key=a`

Supported Special keys

`enter`, `esc`, `back`, `right`, `left`, `up`, `down`, `shift` and `win` (windows key)

Note: Use this if you want to test if the Sparkplug is getting detected as a keyboard.