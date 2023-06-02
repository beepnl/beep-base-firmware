v1.5.13 adds support for an external real-time clock (RTC). 

When using an external RTC, the logged measurement data from your BEEP base will have correct timestamps even when the base is offline and disconnected from the LoraWAN network. 
This will ensure that the timestamps match when [importing and merging](https://beepsupport.freshdesk.com/en/support/solutions/articles/60000697129-download-beep-base-data-through-bluetooth) the logged data afterwards.

It is recommended to use the Maxim DS3231 real-time clock with a coin-cell battery for backup power ([available here](https://www.tinytronics.nl/shop/nl/sensoren/tijd/rtc-ds3231m-module-i2c)). 
Connect the RTC module to the I2C screw-terminal on the BEEP base PCB as shown:

![DS3231 module connected to the I2C screw terminal](/../external-RTC/Source_V1.5.13/Docs/ds3231_module.png?raw=true)
