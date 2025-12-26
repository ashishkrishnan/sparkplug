### POSIX Supported Timezones

#### North America
| Region | Description | POSIX TZ String |
| :--- | :--- | :--- |
| **US Eastern** | New York, Toronto | `EST5EDT,M3.2.0,M11.1.0` |
| **US Central** | Chicago, Winnipeg | `CST6CDT,M3.2.0,M11.1.0` |
| **US Mountain** | Denver, Edmonton | `MST7MDT,M3.2.0,M11.1.0` |
| **US Pacific** | Los Angeles, Vancouver | `PST8PDT,M3.2.0,M11.1.0` |
| **US Arizona** | Phoenix (No DST) | `MST7` |
| **US Alaska** | Anchorage | `AKST9AKDT,M3.2.0,M11.1.0` |
| **US Hawaii** | Honolulu (No DST) | `HST10` |

#### Europe
| Region | Description | POSIX TZ String |
| :--- | :--- | :--- |
| **UK / Ireland** | London, Dublin | `GMT0BST,M3.5.0/1,M10.5.0` |
| **Central Europe** | Berlin, Paris, Rome | `CET-1CEST,M3.5.0,M10.5.0/3` |
| **Eastern Europe** | Athens, Helsinki | `EET-2EEST,M3.5.0/3,M10.5.0/4` |
| **Russia** | Moscow (No DST) | `MSK-3` |
| **UTC** | Coordinated Universal Time | `UTC0` |

#### Asia & Pacific
| Region | Description | POSIX TZ String |
| :--- | :--- | :--- |
| **India** | Bengaluru, New Delhi | `IST-5:30` |
| **China** | Beijing, Shanghai | `CST-8` |
| **Japan** | Tokyo | `JST-9` |
| **Singapore** | Singapore | `SGT-8` |
| **South Korea** | Seoul | `KST-9` |
| **Australia East**| Sydney, Melbourne | `AEST-10AEDT,M10.1.0,M4.1.0/3` |
| **Australia West**| Perth (No DST) | `AWST-8` |
| **New Zealand** | Auckland | `NZST-12NZDT,M9.5.0,M4.1.0/3` |


### Understanding the Syntax

If you need to create a custom string, the format follows this POSIX standard:

`STDoffset[DST[offset],start[/time],end[/time]]`

* **STD**: Name of standard time (e.g., `EST`, `IST`).
* **offset**: Hours added to local time to get UTC.
    * **Positive (+)** for West of UTC.
    * **Negative (-)** for East of UTC.
* **DST**: *(Optional)* Name of daylight saving time. If missing, DST is not observed.
* **start/end**: When DST starts and ends.
    * Format: `Mm.w.d` (Month, week, day)
    * Example `M3.2.0`: Month 3 (March), 2nd week, 0 (Sunday).