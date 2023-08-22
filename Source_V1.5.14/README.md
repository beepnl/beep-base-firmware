v1.5.14 adds a status indicator for the external real-time clock (RTC). If a DS3231 RTC is detected and in use, the timestamp is now prepended with 0x26 in the payload as well as in the flash log. If there is no DS3231 RTC detected, the timestamp is prepended with 0x25 (as in previous firmware versions).